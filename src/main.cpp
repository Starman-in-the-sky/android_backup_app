#include "mainwindow.h"
#include "DevicePage.h"
#include "HistoryPage.h"
#include "SettingsPage.h"
#include "ExplorerWindows.h"
#include <filesystem>
#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "TextureLoader.h"
#include <Windows.h>
#include <cassert>

#pragma execution_character_set("utf-8")

namespace fs = std::filesystem;

enum AppState {
    APP_DEVICE,
    APP_HISTORY,
    APP_SETTINGS,
    APP_EXPLORER
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (!glfwInit())
        return -1;

    // Устанавливаем текущий рабочий каталог на путь проекта
    fs::path projectPath = "E:/Work/Projects/University Projects/plusi_kursach/Phone_backup_app";
    fs::current_path(projectPath);
    std::cout << "Current path set to: " << fs::current_path() << std::endl;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Phone Backup App", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Запрещаем изменение размера окна
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    AppState currentState = APP_DEVICE;

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.988f, 0.988f, 0.988f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.086f, 0.466f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.086f, 0.466f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2588f, 0.5882f, 0.9804f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4078f, 0.6745f, 0.9922f, 1.0f);

    // Используем относительные пути для шрифтов и изображений
    fs::path fontPath = fs::current_path() / "fonts" / "OpenSans-Regular.ttf";
    std::cout << "Font path: " << fontPath << std::endl; // Добавляем вывод пути шрифта для отладки
    assert(fs::exists(fontPath) && "Font file does not exist");

    ImFont* fontOpenSans18 = io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    assert(fontOpenSans18 && "Failed to load font");
    ImFont* fontOpenSans32 = io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 32.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    assert(fontOpenSans32 && "Failed to load font");

    ImFontConfig config;
    config.MergeMode = true;
    static const ImWchar ranges[] = { 0x0020, 0x00FF, 0x0400, 0x052F, 0 };
    io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 18.0f, &config, ranges);
    io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 32.0f, &config, ranges);

    io.Fonts->Build();

    GLuint iconDevice = loadIconTexture((fs::current_path() / "assets" / "iphone.png").string().c_str());
    GLuint iconBackup = loadIconTexture((fs::current_path() / "assets" / "database.png").string().c_str());
    GLuint iconSettings = loadIconTexture((fs::current_path() / "assets" / "gear.png").string().c_str());

    ExplorerWindows explorer;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.988f, 0.988f, 0.988f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Navigation bar
        ImGui::PushFont(fontOpenSans32);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Файл")) {
                ImGui::PushFont(fontOpenSans18);
                if (ImGui::MenuItem("Открыть", "Ctrl+O")) { /* Open file */ }
                if (ImGui::MenuItem("Сохранить", "Ctrl+S")) { /* Save file */ }
                if (ImGui::MenuItem("Выйти", "Ctrl+W")) { glfwSetWindowShouldClose(window, GLFW_TRUE); }
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            ImGui::PopFont();

            ImGui::PushFont(fontOpenSans32);
            if (ImGui::Button("Моё устройство")) {
                currentState = APP_DEVICE;
            }
            ImGui::PopFont();

            ImGui::PushFont(fontOpenSans32);
            if (ImGui::Button("История бэкапов")) {
                currentState = APP_HISTORY;
            }
            ImGui::PopFont();

            ImGui::PushFont(fontOpenSans32);
            if (ImGui::Button("Настройки")) {
                currentState = APP_SETTINGS;
            }
            ImGui::PopFont();

            ImGui::PushFont(fontOpenSans32);
            if (ImGui::Button("Файловый менеджер")) {
                currentState = APP_EXPLORER;
            }
            ImGui::PopFont();

            ImGui::EndMainMenuBar();
        }

        switch (currentState) {
        case APP_DEVICE:
            ShowDevicePage(explorer.isDeviceConnected(), fontOpenSans32, &explorer);
            break;
        case APP_HISTORY:
            ShowHistoryPage();
            break;
        case APP_SETTINGS:
            ShowSettingsPage();
            break;
        case APP_EXPLORER:
            explorer.drawFilesystem(fontOpenSans18, fontOpenSans32);
            break;
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
