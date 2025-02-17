#include "constant-buffers.h"

#include "graphics/render-command.h"
#include "graphics/constant-buffer.h"

void ConstantBuffers::init(RenderCommand& rcommand, Materials& materials, Lights& lights) {
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_NI_MESH)) = rcommand.createConstantBuffer("perNiMesh", 0, sizeof(cb::perNiMesh));
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_FRAME)) = rcommand.createConstantBuffer("perFrame", 1, sizeof(cb::perFrame));
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_MATERIAL_CHANGE)) = rcommand.createConstantBuffer("perMaterialChange", 2, materials.byteWidth());
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_LIGHT_CHANGE)) = rcommand.createConstantBuffer("perLightChange", 3, lights.byteWidth());
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_SHADOW_PASS)) = rcommand.createConstantBuffer("perShadowPass", 4, sizeof(cb::perLightChange::perShadowPass));
}

void ConstantBuffers::destroy(RenderCommand& rcommand) {
    for (auto& cb : m_cbs) {
        rcommand.deleteConstantBuffer(cb);
    }
}
