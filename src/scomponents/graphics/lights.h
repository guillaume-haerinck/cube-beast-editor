#pragma once

#include <vector>

#include "graphics/constant-buffer.h"

class Lights {
public:
    Lights() {
        loadDefaults();
    };

    size_t directionalsCapacity() const { return m_directionalsMaxSize; }
    size_t pointsCapacity() const { return m_pointsMaxSize; }
    size_t spotsCapacity() const { return m_spotsMaxSize; }

    unsigned int byteWidth() const { return directionalsByteWidth() + pointsByteWidth() + spotsByteWidth(); }
    unsigned int directionalsByteWidth() const { return sizeof(cb::perLightChange::DirectionalLight) * m_directionalsMaxSize; }
    unsigned int pointsByteWidth() const { return sizeof(cb::perLightChange::PointLight) * m_pointsMaxSize; }
    unsigned int spotsByteWidth() const { return sizeof(cb::perLightChange::SpotLight) * m_spotsMaxSize; }

    const cb::perLightChange::DirectionalLight* const directionalsData() { return m_directionals.data(); }
    const cb::perLightChange::PointLight* const pointsData() { return m_points.data(); }
    const cb::perLightChange::SpotLight* const spotsData() { return m_spots.data(); }

    bool hasToBeUpdated() const { return m_hasToBeUpdated; }

private:
    void loadDefaults();

private:
    std::vector<cb::perLightChange::DirectionalLight> m_directionals;
    std::vector<cb::perLightChange::PointLight> m_points;
    std::vector<cb::perLightChange::SpotLight> m_spots;

    unsigned int m_directionalsMaxSize = 1;
    unsigned int m_pointsMaxSize = 0;
    unsigned int m_spotsMaxSize = 0;

    bool m_hasToBeUpdated = true;

private:
    friend class RenderSystem;
};
