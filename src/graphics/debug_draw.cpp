#include "debug-draw.h"

#include "graphics/constant-buffer.h"

DebugDraw::DebugDraw(RenderCommand& rcommand, SingletonComponents& scomps) 
    : m_rcommand(rcommand), m_scomps(scomps)
{
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::drawLines(const std::vector<glm::vec3>& lines) {
    m_rcommand.disableDepthTest();
    m_rcommand.unbindRenderTargets();
    m_rcommand.bindVertexBuffer(m_scomps.ddrawVb);
    m_rcommand.bindPipeline(m_scomps.pipelines.at(scomp::PipelineIndex::PIP_DDRAW));

    // Update per non-instanced mesh constant buffer
    // TODO handle render in render system ?
    {
        cb::perNiMesh cbData;
        scomp::ConstantBuffer& perNiMeshCB = m_scomps.constantBuffers.at(scomp::ConstantBufferIndex::PER_NI_MESH);
        cbData.matWorld = glm::mat4(1);
        m_rcommand.updateConstantBuffer(perNiMeshCB, &cbData);
    }

    m_rcommand.drawLines(2);
}
