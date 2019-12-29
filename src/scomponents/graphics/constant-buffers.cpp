#include "constant-buffers.h"

#include "graphics/render-command.h"
#include "graphics/constant-buffer.h"

void ConstantBuffers::init(RenderCommand& rcommand) {
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_NI_MESH)) = rcommand.createConstantBuffer("perNiMesh", 0, sizeof(cb::perNiMesh));
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_FRAME)) = rcommand.createConstantBuffer("perFrame", 1, sizeof(cb::perFrame));

    // TODO use arrays
    // TEMP
    const unsigned int minArraySize = 5;
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_MATERIAL_CHANGE)) = rcommand.createConstantBuffer("perMaterialChange", 2, sizeof(cb::perMaterialChange) * minArraySize);
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_LIGHT_CHANGE)) = rcommand.createConstantBuffer("perLightChange", 3, sizeof(cb::perLightChange));
}

void ConstantBuffers::destroy(RenderCommand& rcommand) {
    for (auto& cb : m_cbs) {
        rcommand.deleteConstantBuffer(cb);
    }
}
