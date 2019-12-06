#include "cube-factory.h"

CubeFactory::CubeFactory(Context& context) : m_ctx(context) {
    m_pipeline = m_ctx.rcommand.createPipeline("res/shaders/basics/basic.vert", "res/shaders/basics/basic.frag");
}

CubeFactory::~CubeFactory()
{
}

met::entity CubeFactory::createCube(const comp::Position& position) {
    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, m_material);
    m_ctx.registry.assign<comp::Pipeline>(entity, m_pipeline);
    m_ctx.registry.assign<comp::Position>(entity, position);
    return entity;
}
