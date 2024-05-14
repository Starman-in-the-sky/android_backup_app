#pragma once

#include <GLFW/glfw3.h> 
#include "imgui.h"


GLuint loadTexture(const char* imagePath);

GLuint loadIconTexture(const char* filename);

bool IconButton(int icon_texture_id, const ImVec2& size);

bool IconButtonWithText(int icon_texture_id, const char* label, const ImVec2& icon_size);