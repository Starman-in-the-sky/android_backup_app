#include "ExplorerWindows.h"
#include <cstdlib>
#include <windows.h>
#include "imgui.h"
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

ExplorerWindows::ExplorerWindows() : currentPath(fs::current_path())
{
    fileExt.insert(".png");
    fileExt.insert(".jpg");
    fileExt.insert(".jpeg");
    fileExt.insert(".bmp");
}

void ExplorerWindows::openImage(const fs::path& path)
{
    std::string command = "start " + path.string();
    std::system(command.c_str());
}

std::string ExplorerWindows::execCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        throw std::runtime_error("Failed to create pipe");
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    if (!CreateProcessA(NULL, const_cast<LPSTR>(cmd.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        throw std::runtime_error("Failed to create process");
    }

    CloseHandle(hWrite);
    DWORD bytesRead;
    while (ReadFile(hRead, buffer.data(), buffer.size(), &bytesRead, NULL)) {
        result.append(buffer.data(), bytesRead);
    }

    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return result;
}

std::vector<std::string> ExplorerWindows::getAdbDevices() {
    std::vector<std::string> devices;
    std::string output = execCommand("adb devices");

    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("device") != std::string::npos && line.find("List of devices attached") == std::string::npos) {
            std::string deviceID = line.substr(0, line.find("\t"));
            devices.push_back(deviceID);
        }
    }
    return devices;
}

void ExplorerWindows::updateDrivesAndDevices() {
    logicalDrives.clear();
    adbDevices.clear();

    // Get logical drives
    DWORD drives = GetLogicalDrives();
    char driveLetter = 'A';
    while (drives) {
        if (drives & 1) {
            char driveRoot[4] = { driveLetter, ':', '\\', '\0' };
            logicalDrives.push_back(driveRoot);
        }
        drives >>= 1;
        driveLetter++;
    }

    // Get ADB devices
    adbDevices = getAdbDevices();
}

bool ExplorerWindows::isDeviceConnected() {
    updateDrivesAndDevices();
    return !adbDevices.empty();
}

std::pair<float, float> ExplorerWindows::getMemoryInfo() {
    std::string output = execCommand("adb shell cat /proc/meminfo");
    std::istringstream stream(output);
    std::string line;
    float totalMem = 0.0f, freeMem = 0.0f;

    while (std::getline(stream, line)) {
        if (line.find("MemTotal:") != std::string::npos) {
            std::istringstream(line.substr(line.find(":") + 1)) >> totalMem;
        }
        if (line.find("MemAvailable:") != std::string::npos) {
            std::istringstream(line.substr(line.find(":") + 1)) >> freeMem;
        }
    }
    totalMem /= 1024.0f * 1024.0f; // Convert to GB
    freeMem /= 1024.0f * 1024.0f; // Convert to GB
    return { freeMem, totalMem };
}

std::vector<std::string> ExplorerWindows::getAndroidDirectoryEntries(const std::string& path) {
    std::vector<std::string> entries;
    std::string output = execCommand("adb shell ls " + path);
    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line)) {
        entries.push_back(line);
    }
    return entries;
}

void ExplorerWindows::drawFilesystem(ImFont* font, ImFont* font32)
{
    // Обновляем список логических дисков и устройств перед отрисовкой
    updateDrivesAndDevices();

    ImGui::SetNextWindowPos(ImVec2(0, 38)); // Статическая позиция (с учетом высоты меню бара)
    ImGui::SetNextWindowSize(ImVec2(900, 500));
    if (ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoCollapse ))
    {
        std::stringstream ss(currentPath.string());
        std::string s;
        pathTmp.clear();
        while (getline(ss, s, '/'))
        {
            if (s != "") pathTmp.push_back(s);
        }

        ImGui::PushFont(font);
        ImGui::BeginChild("PathToFile", ImVec2(-1, 40), true, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::Button("/"))
        {
            currentPath = "/";
        }
        for (size_t i = 0; i < pathTmp.size(); i++)
        {
            ImGui::SameLine();
            if (ImGui::Button(pathTmp.at(i).c_str()))
            {
                fs::path newPath;
                for (size_t j = 0; j <= i; j++)
                {
                    newPath /= pathTmp[j];
                }
                currentPath = newPath;
            }
            ImGui::SameLine();
            ImGui::Text("/");
        }
        ImGui::SetScrollHereX(1.0f);
        ImGui::EndChild();
        ImGui::PopFont();

        ImGui::PushFont(font);
        ImGui::Columns(2, nullptr, false); // Устанавливаем две колонки
        ImGui::SetColumnOffset(1, 210); // Устанавливаем смещение второй колонки

        // Левая колонка
        ImGui::BeginChild("LeftPane", ImVec2(200, 0), true);

        for (const auto& drive : logicalDrives) {
            ImGui::PushFont(font);
            if (ImGui::Button(drive.c_str()))
            {
                currentPath = drive;
            }
            ImGui::PopFont();
        }

        for (const auto& device : adbDevices) {
            ImGui::PushFont(font);
            if (ImGui::Button(("Android Device: " + device).c_str()))
            {
                currentPath = "/mnt/sdcard"; // Пример пути к внутреннему хранилищу устройства
            }
            ImGui::PopFont();
        }

        ImGui::EndChild();

        ImGui::NextColumn(); // Переключаемся на следующую колонку

        // Правая колонка
        if (ImGui::BeginChild("fs", ImVec2(0, 0), true))
        {
            ImGui::GetStyle().ButtonTextAlign = ImVec2(0.0f, 0.5f);
            float buttonWidth = ImGui::GetWindowSize().x - ImGui::GetStyle().ScrollbarSize - ImGui::GetStyle().WindowPadding.x * 2;
            if (currentPath.string().find("/mnt/sdcard") != std::string::npos) {
                // Если мы находимся в Android устройстве, используем ADB для получения списка файлов
                auto directoryEntries = getAndroidDirectoryEntries(currentPath.string());
                for (const auto& entry : directoryEntries) {
                    if (entry.empty()) continue;
                    if (entry.back() == '/') {
                        // Это директория
                        if (ImGui::Button(entry.c_str(), ImVec2(buttonWidth, 25))) {
                            currentPath /= entry.substr(0, entry.size() - 1); // Удаляем последний символ '/'
                        }
                    }
                    else {
                        // Это файл
                        if (fileExt.find(fs::path(entry).extension().string()) != fileExt.end()) {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.714f, 0.0f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.714f, 0.0f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.56f, 0.814f, 0.0f, 1.0f));
                            if (ImGui::Button(entry.c_str(), ImVec2(buttonWidth, 25))) openImage(currentPath / entry);
                            ImGui::PopStyleColor(3);
                        }
                        else {
                            ImGui::Button(entry.c_str(), ImVec2(buttonWidth, 25));
                        }
                    }
                }
            }
            else {
                // Обычный режим работы с файлами
                for (const auto& item : fs::directory_iterator(currentPath)) {
                    if (item.path().filename().c_str()[0] == '.') continue;

                    if (fs::is_directory(item)) {
                        if (ImGui::Button(item.path().filename().string().c_str(), ImVec2(buttonWidth, 25))) {
                            currentPath = item;
                        }
                    }
                    else if (fs::is_regular_file(item)) {
                        if (fileExt.find(item.path().extension().string()) != fileExt.end()) {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.714f, 0.0f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.714f, 0.0f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.56f, 0.814f, 0.0f, 1.0f));
                            if (ImGui::Button(item.path().filename().string().c_str(), ImVec2(buttonWidth, 25))) openImage(item.path());
                            ImGui::PopStyleColor(3);
                        }
                        else {
                            ImGui::Button(item.path().filename().string().c_str(), ImVec2(buttonWidth, 25));
                        }
                    }
                }
            }
            ImGui::EndChild();
        }
        ImGui::PopFont();

        ImGui::End();
    }
}
