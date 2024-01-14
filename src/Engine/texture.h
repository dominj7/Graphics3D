#pragma once
#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "stb/stb_image.h"
#include "Application/utils.h"

namespace xe {
    GLuint create_texture(const std::string &name, bool is_sRGB = true);
}
