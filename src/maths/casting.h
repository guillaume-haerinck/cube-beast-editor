#pragma once

#include <glm/glm.hpp>

namespace voxmt {
    glm::vec2 uintBitsToFloat2(unsigned int value);

    unsigned int float2BitsToUInt(const glm::vec2& value);

}
