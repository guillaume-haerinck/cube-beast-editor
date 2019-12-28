#pragma once

#include <glm/glm.hpp>

class Viewport {
private:
    glm::ivec2 m_viewportSize = { 600, 600 };
    glm::ivec2 m_viewportPosTopLeft = { 0, 0 };
    bool m_isViewportHovered = false;
};
