#include "casting.h"

namespace voxmt {
    glm::vec3 intToNormColor(int value) {
        glm::vec3 result;
        result.r = value & 0xFF;
        result.g = (value >> 8) & 0xFF;
        result.b = (value >> 16) & 0xFF;
        return result / 255.0f;
    }

    int colorToInt(unsigned char r, unsigned char g, unsigned char b) {
        return r | (g << 8) | (b << 16);
    }
}
