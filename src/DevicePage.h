#ifndef DEVICEPAGE_H
#define DEVICEPAGE_H

#include <imgui.h>
#include "ExplorerWindows.h"

/**
 * @brief Настраивает страницу устройства, загружая необходимые ресурсы.
 * @param isDeviceConnected Флаг, указывающий, подключено ли устройство.
 */
void SetupDevicePage(bool isDeviceConnected);

/**
 * @brief Отображает страницу устройства в окне ImGui.
 * @param isDeviceConnected Флаг, указывающий, подключено ли устройство.
 * @param font Шрифт для текста.
 * @param explorer Указатель на объект ExplorerWindows для получения информации о памяти устройства.
 */
void ShowDevicePage(bool isDeviceConnected, ImFont* font, ExplorerWindows* explorer);

#endif // DEVICEPAGE_H
