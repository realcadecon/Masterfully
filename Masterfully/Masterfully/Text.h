#pragma once
#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "Program.h"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

namespace Text {
    void loadCharacters(FT_Face, std::map<char, Character>&);
    void renderText(std::shared_ptr<Program> s, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int& VAO, unsigned int& VBO, std::map<char, Character>& characters, GLFWwindow* window);
}

#endif