#include "lights.h"

#include <cassert>

void Lights::loadDefaults() {
    cb::perLightChange::DirectionalLight dirLight;
    dirLight.color = glm::vec3(1, 1, 1);
    dirLight.direction = glm::vec3(-1.0, -0.5, -0.5);
    dirLight.intensity = 1;
    m_directionals.push_back(dirLight);

    assert(m_directionals.size() <= m_directionalsMaxSize);
    assert(m_points.size() <= m_pointsMaxSize);
    assert(m_spots.size() <= m_spotsMaxSize);
}
