#ifndef EXPLORERWINDOWS_H
#define EXPLORERWINDOWS_H

#include <filesystem>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <set>

namespace fs = std::filesystem;

/**
 * @class ExplorerWindows
 * @brief ����� ��� ���������� �������� �������� � ������������� ������������ � Windows.
 */
class ExplorerWindows
{
public:
    /**
     * @brief ����������� ������ ExplorerWindows.
     * �������������� ������� ���� ������� ������� ���������.
     */
    ExplorerWindows();

    /**
     * @brief ������������ �������� ������� � ���� ImGui.
     * @param font ����� ��� ������.
     * @param font32 ������� ����� ��� ������.
     */
    void drawFilesystem(ImFont* font, ImFont* font32);

    /**
     * @brief ��������� ������ ���������� ������ � ������������ ���������.
     */
    void updateDrivesAndDevices();

    /**
     * @brief ���������, ���������� �� Android-����������.
     * @return true, ���� Android-���������� ����������, ����� false.
     */
    bool isDeviceConnected();

    /**
     * @brief �������� ���������� � ������ ����������.
     * @return ���� (freeMem, totalMem), ��� freeMem - ��������� ������, � totalMem - ����� ������ � ����������.
     */
    std::pair<float, float> getMemoryInfo();

private:
    fs::path currentPath; ///< ������� ���� � �������� �������.
    std::vector<std::string> pathTmp; ///< ��������� ���� ��� ���������� �������� ����.
    std::set<std::string> fileExt; ///< ����� ���������� ������ ��� �����������.
    std::vector<std::string> logicalDrives; ///< ������ ���������� ������.
    std::vector<std::string> adbDevices; ///< ������ ������������ Android-���������.

    /**
     * @brief ��������� ����������� �� ���������� ����.
     * @param path ���� � �����������.
     */
    void openImage(const fs::path& path);

    /**
     * @brief ��������� ������� � ��������� ������ � ���������� ���������.
     * @param cmd ������� ��� ����������.
     * @return ��������� ���������� �������.
     */
    std::string execCommand(const std::string& cmd);

    /**
     * @brief �������� ������ ������������ Android-���������.
     * @return ������ �����, ���������� �������������� ���������.
     */
    std::vector<std::string> getAdbDevices();

    /**
     * @brief �������� ������ ������ � ����� � ���������� �� Android-����������.
     * @param path ���� � ���������� �� Android-����������.
     * @return ������ �����, ���������� ����� ������ � �����.
     */
    std::vector<std::string> getAndroidDirectoryEntries(const std::string& path);
};

#endif // EXPLORERWINDOWS_H
