#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "render-command.h"
#include "scomponents/singleton-components.h"
#include "scomponents/graphics/mesh.h"

class DebugDraw {
public:
    DebugDraw(RenderCommand& rcommand, SingletonComponents& scomps);
    ~DebugDraw();

    void addLines(const std::vector<glm::vec3>& lines);
    void updateBuffer();
    unsigned int getCount() const;

private:
    RenderCommand& m_rcommand;
    SingletonComponents& m_scomps;
    std::vector<float> m_positions;
};
