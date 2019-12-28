#pragma once

#include <glm/glm.hpp>

class Viewport {
public:
    Viewport() {};

    const glm::ivec2& size() const { return m_size; }
    const glm::ivec2& posTopLeft() const { return m_posTopLeft; }
    bool isHovered() const { return m_isHovered; }

private:
    glm::ivec2 m_size = { 600, 600 };
    glm::ivec2 m_posTopLeft = { 0, 0 };
    bool m_isHovered = false;

private:
    friend class App;
    friend class ViewportGui;
};
