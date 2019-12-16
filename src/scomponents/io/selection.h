#pragma once

#include <met/met.hpp>
#include <glm/glm.hpp>

namespace scomp {
    enum class Face {
        NONE = 0,
        FRONT,
        RIGHT,
        TOP,
        LEFT,
        BOTTOM,
        BACK
    };

    struct Hovered {
        glm::ivec3 position;
        Face face;
        bool exist = false;
    };
}
