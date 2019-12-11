#pragma once

#include <glm/glm.hpp>

namespace voxmt {
    // Up to 256 * 256 * 256 different entites because 3 color channel
    // http://www.mbsoftworks.sk/tutorials/opengl3/18-3d-picking-pt1/
    glm::vec3 intToNormColor(int value);

    int colorToInt(unsigned char r, unsigned char g, unsigned char b);
}
