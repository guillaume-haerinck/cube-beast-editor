#include "pipelines.h"

#include "graphics/render-command.h"
#include "graphics/constant-buffer.h"

void Pipelines::init(RenderCommand& rcommand, const ConstantBuffers& cbs, const Materials& mats, const Lights& lights) {
    std::vector<ConstantBuffer> usedCbs;

    // Geometry
    usedCbs = {
        cbs.at(ConstantBufferIndex::PER_FRAME),
        cbs.at(ConstantBufferIndex::PER_MATERIAL_CHANGE)
    };
    const char* VSGeo = 
        #include "graphics/shaders/geometry.vert"
    ;
    std::string FSGeo =
        #include "graphics/shaders/geometry.frag"
    ;
    replaceInString(FSGeo, "MAX_COUNT_MATERIALS", std::to_string(mats.capacity()).c_str());
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_GEOMETRY)) = rcommand.createPipeline(VSGeo, FSGeo.c_str(), usedCbs);

    // Shadow map
    usedCbs = {
        cbs.at(ConstantBufferIndex::PER_SHADOW_PASS)
    };
    const char* VSShadowMap = 
        #include "graphics/shaders/shadow-map.vert"
    ;
    const char* FSShadowMap =
        #include "graphics/shaders/shadow-map.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_SHADOW_MAP)) = rcommand.createPipeline(VSShadowMap, FSShadowMap, usedCbs);

    // Grid
    usedCbs = {
        cbs.at(ConstantBufferIndex::PER_FRAME)
    };
    const char* VSGrid = 
        #include "graphics/shaders/grid.vert"
    ;
    const char* FSGrid =
        #include "graphics/shaders/grid.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_GRID)) = rcommand.createPipeline(VSGrid, FSGrid, usedCbs);

    // Debug draw
    usedCbs = {
        cbs.at(ConstantBufferIndex::PER_FRAME)
    };
    const char* VSDdraw = 
        #include "graphics/shaders/ddraw.vert"
    ;
    const char* FSDdraw =
        #include "graphics/shaders/ddraw.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_DDRAW)) = rcommand.createPipeline(VSDdraw, FSDdraw, usedCbs);

    // Gui
    usedCbs = {
        cbs.at(ConstantBufferIndex::PER_NI_MESH),
        cbs.at(ConstantBufferIndex::PER_FRAME)
    };
    const char* VSGui = 
        #include "graphics/shaders/gui.vert"
    ;
    const char* FSGui =
        #include "graphics/shaders/gui.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_GUI)) = rcommand.createPipeline(VSGui, FSGui, usedCbs);

    // Lighting
    usedCbs = {
        cbs.at(ConstantBufferIndex::PER_FRAME),
        cbs.at(ConstantBufferIndex::PER_LIGHT_CHANGE)
    };
    const char* VSLighting = 
        #include "graphics/shaders/lighting.vert"
    ;
    std::string FSLighting =
        #include "graphics/shaders/lighting.frag"
    ;
    replaceInString(FSLighting, "MAX_COUNT_DIR_LIGHTS", std::to_string(lights.directionalsCapacity()).c_str());
    replaceInString(FSLighting, "MAX_COUNT_POINT_LIGHTS", std::to_string(lights.pointsCapacity()).c_str());
    replaceInString(FSLighting, "MAX_COUNT_SPOT_LIGHTS", std::to_string(lights.spotsCapacity()).c_str());
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_LIGHTING)) = rcommand.createPipeline(VSLighting, FSLighting.c_str(), usedCbs, {"g_albedo", "g_normal"});
}

void Pipelines::destroy(RenderCommand& rcommand) {
    for (auto& pip : m_pips) {
        rcommand.deletePipeline(pip);
    }
}

void Pipelines::replaceInString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}
