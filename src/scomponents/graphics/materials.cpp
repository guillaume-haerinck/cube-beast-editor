#include "materials.h"

void Materials::loadDefaultPalette1() {
    m_materials = {
        { glm::vec3(1.0f, 1.0f, 1.0f), 0.0f },
        { glm::vec3(1.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 1.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 1.0f), 0.0f },
        { glm::vec3(0.5f, 0.5f, 0.5f), 0.0f },
        { glm::vec3(1.0f, 1.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 1.0f, 1.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f },
        { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f }
    };

    assert(m_materials.size() < m_maxSize && "Palette1 exceed max size");
};
