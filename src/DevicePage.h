#ifndef DEVICEPAGE_H
#define DEVICEPAGE_H

#include <imgui.h>
#include "ExplorerWindows.h"

void SetupDevicePage(bool isDeviceConnected);
void ShowDevicePage(bool isDeviceConnected, ImFont* font, ExplorerWindows* explorer);

#endif // DEVICEPAGE_H
