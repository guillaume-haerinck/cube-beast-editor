#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "render-command.h"
#include "scomponents/singleton-components.h"

class DebugDraw {
public:
    DebugDraw(RenderCommand& rcommand, SingletonComponents& scomps);
    ~DebugDraw();

    void addLine(const glm::vec3& lineStart, const glm::vec3& lineEnd);
    void updateBuffer();
    unsigned int getCount() const;

private:
    RenderCommand& m_rcommand;
    SingletonComponents& m_scomps;
    std::vector<glm::vec3> m_positions;
};
