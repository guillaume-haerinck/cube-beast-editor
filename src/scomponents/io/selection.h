#pragma once

#include <met/met.hpp>

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
        met::entity id = met::null_entity;
        Face face;
    };
}
