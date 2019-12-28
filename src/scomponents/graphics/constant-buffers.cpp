#include "constant-buffers.h"

#include "graphics/render-command.h"
#include "graphics/constant-buffer.h"

void ConstantBuffers::init(RenderCommand& rcommand) {
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_NI_MESH)) = rcommand.createConstantBuffer("perNiMesh", sizeof(cb::perNiMesh));
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_FRAME)) = rcommand.createConstantBuffer("perFrame", sizeof(cb::perFrame));

    // TODO use arrays
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_MATERIAL_CHANGE)) = rcommand.createConstantBuffer("perMaterialChange", sizeof(cb::perMaterialChange));
    m_cbs.at(static_cast<unsigned int>(ConstantBufferIndex::PER_LIGHT_CHANGE)) = rcommand.createConstantBuffer("perLightChange", sizeof(cb::perLightChange));
}

void ConstantBuffers::destroy(RenderCommand& rcommand) {
    // TODO
}
