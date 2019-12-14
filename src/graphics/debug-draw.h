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

    void drawLines(const std::vector<glm::vec3>& lines);

private:
    RenderCommand& m_rcommand;
    SingletonComponents& m_scomps;
    scomp::VertexBuffer m_vb;
};
