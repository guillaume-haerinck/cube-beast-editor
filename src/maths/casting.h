#pragma once

#include <glm/glm.hpp>

namespace voxmt {
    glm::vec3 intToNormColor(int value);

    int colorToInt(unsigned char r, unsigned char g, unsigned char b);
}
