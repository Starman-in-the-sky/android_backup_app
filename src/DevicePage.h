#ifndef DEVICEPAGE_H
#define DEVICEPAGE_H

#include <imgui.h>
#include "ExplorerWindows.h"

/**
 * @brief ����������� �������� ����������, �������� ����������� �������.
 * @param isDeviceConnected ����, �����������, ���������� �� ����������.
 */
void SetupDevicePage(bool isDeviceConnected);

/**
 * @brief ���������� �������� ���������� � ���� ImGui.
 * @param isDeviceConnected ����, �����������, ���������� �� ����������.
 * @param font ����� ��� ������.
 * @param explorer ��������� �� ������ ExplorerWindows ��� ��������� ���������� � ������ ����������.
 */
void ShowDevicePage(bool isDeviceConnected, ImFont* font, ExplorerWindows* explorer);

#endif // DEVICEPAGE_H
