#include "DevicePage.h"
#include "TextureLoader.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include <filesystem>
#include <iostream>
#include <cassert>

namespace fs = std::filesystem;

static GLuint textureID = 0;
static GLuint iconFileExplorer = 0;
static GLuint iconApps = 0;
static GLuint iconNotes = 0;
static GLuint iconMusic = 0;

void SetupDevicePage(bool isDeviceConnected) {
    fs::path imagePath = fs::current_path() / "assets" / (isDeviceConnected ? "Phone_ON.png" : "Phone_OFF.png");
    std::cout << "Image path: " << imagePath << std::endl;
    assert(fs::exists(imagePath) && "Image file does not exist");

    if (textureID == 0) {
        textureID = loadTexture(imagePath.string().c_str());
    }

    if (iconFileExplorer == 0) {
        iconFileExplorer = loadIconTexture((fs::current_path() / "assets" / "file_explorer.png").string().c_str());
    }

    if (iconApps == 0) {
        iconApps = loadIconTexture((fs::current_path() / "assets" / "gallery.png").string().c_str());
    }

    if (iconNotes == 0) {
        iconNotes = loadIconTexture((fs::current_path() / "assets" / "videos.png").string().c_str());
    }

    if (iconMusic == 0) {
        iconMusic = loadIconTexture((fs::current_path() / "assets" / "music.png").string().c_str());
    }
}

void ShowDevicePage(bool isDeviceConnected, ImFont* font, ExplorerWindows* explorer) {
    ImGui::SetNextWindowPos(ImVec2(0, 38));
    ImGui::SetNextWindowSize(ImVec2(1280, 682));
    ImGui::Begin("WWWWindow", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    SetupDevicePage(isDeviceConnected);
    if (textureID != 0) {
        ImGui::SetCursorPos(ImVec2(100, 52));
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(309, 578));
    }

    ImGui::SameLine();
    ImGui::BeginGroup(); // Начало группы для выравнивания иконок и текста

    // Создаем 2 колонки для иконок и текста
    ImGui::Columns(2, NULL, false);

    // Левая верхняя иконка и текст
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150); // Сдвиг вправо
    ImGui::Image((void*)(intptr_t)iconFileExplorer, ImVec2(128, 128));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255)); // Устанавливаем черный цвет текста
    ImGui::Text("Explorer:");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::NextColumn();

    // Правая верхняя иконка и текст
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
    ImGui::Image((void*)(intptr_t)iconApps, ImVec2(128, 128));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255)); // Устанавливаем черный цвет текста
    ImGui::Text("Pictures:");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::NextColumn();

    // Левая нижняя иконка и текст
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150); // Сдвиг вправо
    ImGui::Image((void*)(intptr_t)iconNotes, ImVec2(128, 128));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255)); // Устанавливаем черный цвет текста
    ImGui::Text("Video:");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::NextColumn();

    // Правая нижняя иконка и текст
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::Image((void*)(intptr_t)iconMusic, ImVec2(128, 128));
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255)); // Устанавливаем черный цвет текста
    ImGui::Text("Music:");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::EndGroup();

    // Поле для заполненности памяти устройства
    auto [freeMem, totalMem] = explorer->getMemoryInfo();
    float usedMem = totalMem - freeMem;
    float usedPercentage = totalMem != 0 ? usedMem / totalMem : 0;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
    ImGui::PushFont(font);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255)); // Устанавливаем черный цвет текста
    ImGui::Text("Internal storage");
    ImGui::ProgressBar(usedPercentage, ImVec2(300, 20));
    ImGui::Text("Available: %.2fG", freeMem);
    ImGui::Text("Total space: %.2fG", totalMem);
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::End();
}
