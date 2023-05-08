#pragma once

#include <glad/glad.h>
#include "../../Render/Texture2D.hpp"

inline Render::Texture2D* DefaultTexture()
{
    auto texture = new Render::Texture2D();
    texture->Wrap_S = GL_CLAMP_TO_BORDER;
    texture->Wrap_T = GL_CLAMP_TO_BORDER;
    texture->Filter_Max = GL_NEAREST;
    texture->Filter_Min = GL_NEAREST;
    texture->Internal_Format = GL_R8UI;
    texture->Image_Format = GL_RED_INTEGER;
    return texture;
}