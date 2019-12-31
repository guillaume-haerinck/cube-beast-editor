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

    glm::ivec3 roundUpFloat3(const glm::vec3& values) {
        return { static_cast<int>(values.x), static_cast<int>(values.y), static_cast<int>(values.z)};
    }

    unsigned int roundUpToNextPowOf2(unsigned int value) {
        value--;
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        value++;
        return value;
    }
}
