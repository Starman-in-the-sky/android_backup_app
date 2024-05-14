#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image.h"  
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLuint loadTexture(const char* imagePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << imagePath << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = channels == 1 ? GL_RED : channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return textureID;
}

GLuint loadIconTexture(const char* filename) {
    return loadTexture(filename);
}

bool IconButton(int icon_texture_id, const ImVec2& size) {
    ImGui::PushID(icon_texture_id);
    bool clicked = ImGui::ImageButton((void*)(intptr_t)icon_texture_id, size);
    ImGui::PopID();
    return clicked;
}

bool IconButtonWithText(int icon_texture_id, const char* label, const ImVec2& icon_size) {
    ImGui::PushID(icon_texture_id);

    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 totalSize = ImVec2(icon_size.x + textSize.x + ImGui::GetStyle().ItemSpacing.x, std::max(icon_size.y, textSize.y));

    bool pressed = ImGui::InvisibleButton("##btn", totalSize);
    if (ImGui::IsItemActive() && ImGui::IsItemHovered()) {
        ImGui::SetItemDefaultFocus();
    }

    ImVec2 cursorPos = ImGui::GetItemRectMin();
    ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + (totalSize.y - icon_size.y) * 0.5f));
    ImGui::Image((void*)(intptr_t)icon_texture_id, icon_size);
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(cursorPos.x + icon_size.x + ImGui::GetStyle().ItemSpacing.x, cursorPos.y + (totalSize.y - textSize.y) * 0.5f));
    ImGui::TextUnformatted(label);

    ImGui::PopID();
    return pressed;
}
