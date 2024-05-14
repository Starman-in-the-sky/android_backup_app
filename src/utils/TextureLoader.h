#pragma once

#include <GLFW/glfw3.h> 
#include "imgui.h"

/**
 * @brief ��������� �������� �� ����� �����������.
 * @param imagePath ���� � ����� �����������.
 * @return ������������� ����������� ��������.
 */
GLuint loadTexture(const char* imagePath);

/**
 * @brief ��������� �������� ������ �� ����� �����������.
 * @param filename ���� � ����� ����������� ������.
 * @return ������������� ����������� �������� ������.
 */
GLuint loadIconTexture(const char* filename);

/**
 * @brief ������� ������ � �������.
 * @param icon_texture_id ������������� �������� ������.
 * @param size ������ ������.
 * @return true, ���� ������ ������, ����� false.
 */
bool IconButton(int icon_texture_id, const ImVec2& size);

/**
 * @brief ������� ������ � ������� � �������.
 * @param icon_texture_id ������������� �������� ������.
 * @param label ����� ��� ����������� ����� � �������.
 * @param icon_size ������ ������.
 * @return true, ���� ������ ������, ����� false.
 */
bool IconButtonWithText(int icon_texture_id, const char* label, const ImVec2& icon_size);
