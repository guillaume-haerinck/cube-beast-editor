#include "debug-draw.h"

#include "graphics/constant-buffer.h"

DebugDraw::DebugDraw(RenderCommand& rcommand, SingletonComponents& scomps) 
    : m_rcommand(rcommand), m_scomps(scomps)
{
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::addLine(const glm::vec3& lineStart, const glm::vec3& lineEnd) {
    m_positions.push_back(lineStart);
    m_positions.push_back(lineEnd);
}

void DebugDraw::updateBuffer() {
    m_rcommand.updateAttributeBuffer(m_scomps.ddrawVb.buffers.at(0), m_positions.data(), sizeof(glm::vec3) * getCount());
}

unsigned int DebugDraw::getCount() const {
    return static_cast<unsigned int>(m_positions.size());
}
