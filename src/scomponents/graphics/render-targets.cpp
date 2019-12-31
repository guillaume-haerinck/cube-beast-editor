#include "render-targets.h"

#include "graphics/render-command.h"
#include "maths/casting.h"

void RenderTargets::init(RenderCommand& rcommand, const Viewport& viewport) {
    PipelineOutputDescription outputDescription = {
        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" },
        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_LightSpacePosition" },
        { RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor" },
        { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
    };
    m_rts.at(static_cast<unsigned int>(RenderTargetIndex::RTT_GEOMETRY)) = rcommand.createRenderTarget(outputDescription, viewport.size());

    glm::ivec2 shadowMapSize = viewport.size();

// Temp as do not match window
#ifdef __EMSCRIPTEN__
    shadowMapSize = glm::ivec2(voxmt::roundUpToNextPowOf2(viewport.size().x), voxmt::roundUpToNextPowOf2(viewport.size().y));
#endif

    outputDescription = {
        { RenderTargetUsage::Depth, RenderTargetType::Texture, RenderTargetDataType::USHORT, RenderTargetChannels::R, "ShadowMap" }
    };
    m_rts.at(static_cast<unsigned int>(RenderTargetIndex::RTT_SHADOW_MAP)) = rcommand.createRenderTarget(outputDescription, shadowMapSize);

    outputDescription = {
        { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Color" },
        { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
    };
    m_rts.at(static_cast<unsigned int>(RenderTargetIndex::RTT_FINAL)) = rcommand.createRenderTarget(outputDescription, viewport.size());
}

void RenderTargets::destroy(RenderCommand& rcommand) {
    for (auto& rt : m_rts) {
        rcommand.deleteRenderTarget(rt);
    }
}
