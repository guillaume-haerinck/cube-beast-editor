#include "pipelines.h"

#include "graphics/render-command.h"
#include "graphics/constant-buffer.h"

void Pipelines::init(RenderCommand& rcommand) {
    std::vector<ConstantBufferIndex> cbIndices;

    // Geometry
    cbIndices = {
        ConstantBufferIndex::PER_FRAME
    };
    const char* VSGeo = 
        #include "graphics/shaders/geometry.vert"
    ;
    const char* FSGeo =
        #include "graphics/shaders/geometry.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_GEOMETRY)) = rcommand.createPipeline(VSGeo, FSGeo, cbIndices);

    // Grid
    cbIndices = {
        ConstantBufferIndex::PER_FRAME
    };
    const char* VSGrid = 
        #include "graphics/shaders/grid.vert"
    ;
    const char* FSGrid =
        #include "graphics/shaders/grid.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_GRID)) = rcommand.createPipeline(VSGrid, FSGrid, cbIndices);

    // Debug draw
    cbIndices = {
        ConstantBufferIndex::PER_FRAME
    };
    const char* VSDdraw = 
        #include "graphics/shaders/ddraw.vert"
    ;
    const char* FSDdraw =
        #include "graphics/shaders/ddraw.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_DDRAW)) = rcommand.createPipeline(VSDdraw, FSDdraw, cbIndices);

    // Gui
    cbIndices = {
        ConstantBufferIndex::PER_FRAME,
        ConstantBufferIndex::PER_NI_MESH
    };
    const char* VSGui = 
        #include "graphics/shaders/gui.vert"
    ;
    const char* FSGui =
        #include "graphics/shaders/gui.frag"
    ;
    // FIXME perNIMesh overrided by PerWindowChange
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_GUI)) = rcommand.createPipeline(VSGui, FSGui, cbIndices);

    // Lighting
    cbIndices = {
        ConstantBufferIndex::PER_FRAME
    };
    const char* VSLighting = 
        #include "graphics/shaders/lighting.vert"
    ;
    const char* FSLighting =
        #include "graphics/shaders/lighting.frag"
    ;
    m_pips.at(static_cast<unsigned int>(PipelineIndex::PIP_LIGHTING)) = rcommand.createPipeline(VSLighting, FSLighting, cbIndices, {"g_albedo", "g_normal", "g_worldPosition"});
}
