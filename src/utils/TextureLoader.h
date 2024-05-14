#pragma once

#include <GLFW/glfw3.h> 
#include "imgui.h"

/**
 * @brief Загружает текстуру из файла изображения.
 * @param imagePath Путь к файлу изображения.
 * @return Идентификатор загруженной текстуры.
 */
GLuint loadTexture(const char* imagePath);

/**
 * @brief Загружает текстуру иконки из файла изображения.
 * @param filename Путь к файлу изображения иконки.
 * @return Идентификатор загруженной текстуры иконки.
 */
GLuint loadIconTexture(const char* filename);

/**
 * @brief Создает кнопку с иконкой.
 * @param icon_texture_id Идентификатор текстуры иконки.
 * @param size Размер кнопки.
 * @return true, если кнопка нажата, иначе false.
 */
bool IconButton(int icon_texture_id, const ImVec2& size);

/**
 * @brief Создает кнопку с иконкой и текстом.
 * @param icon_texture_id Идентификатор текстуры иконки.
 * @param label Текст для отображения рядом с иконкой.
 * @param icon_size Размер иконки.
 * @return true, если кнопка нажата, иначе false.
 */
bool IconButtonWithText(int icon_texture_id, const char* label, const ImVec2& icon_size);
