#ifndef EXPLORERWINDOWS_H
#define EXPLORERWINDOWS_H

#include <filesystem>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <set>

namespace fs = std::filesystem;

class ExplorerWindows
{
public:
    ExplorerWindows();
    void drawFilesystem(ImFont* font, ImFont* font32);
    void updateDrivesAndDevices();
    bool isDeviceConnected(); 
    std::pair<float, float> getMemoryInfo(); 

private:
    fs::path currentPath;
    std::vector<std::string> pathTmp;
    std::set<std::string> fileExt;
    std::vector<std::string> logicalDrives;
    std::vector<std::string> adbDevices;

    void openImage(const fs::path& path);
    std::string execCommand(const std::string& cmd);
    std::vector<std::string> getAdbDevices();
};

#endif // EXPLORERWINDOWS_H
