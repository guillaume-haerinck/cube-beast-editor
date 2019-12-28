#pragma once

#include <met/met.hpp>
#include <glm/glm.hpp>

enum class Face {
    NONE = 0,
    FRONT,
    RIGHT,
    TOP,
    LEFT,
    BOTTOM,
    BACK
};

class Hovered {
public:
    Hovered() {};

    const glm::ivec3& position() const { return m_position; }
    Face face() const { return m_face; }
    bool exist() const { return m_exist; }
    bool isCube() const { return m_isCube; }
    met::entity id() const { return m_id; }

private:
    glm::ivec3 m_position;
    Face m_face = Face::NONE;
    bool m_exist = false;
    bool m_isCube = false;
    met::entity m_id = met::null;

private:
    friend class SelectionSystem;
};

