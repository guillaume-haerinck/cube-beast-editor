#include "debug-draw.h"

#include "graphics/constant-buffer.h"

DebugDraw::DebugDraw(RenderCommand& rcommand, SingletonComponents& scomps) 
    : m_rcommand(rcommand), m_scomps(scomps)
{
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::addLines(const std::vector<glm::vec3>& lines) {
    // TODO
}

void DebugDraw::updateBuffer() {
    // TODO
}

unsigned int DebugDraw::getCount() const {
    return static_cast<unsigned int>(m_positions.size());
}
