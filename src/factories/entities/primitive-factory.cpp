#include "primitive-factory.h"

PrimitiveFactory::PrimitiveFactory(Context& context) : m_ctx(context) {
    m_pipeline = m_ctx.rcommand.createPipeline("res/shaders/basics/basic.vert", "res/shaders/basics/basic.frag");
}

PrimitiveFactory::~PrimitiveFactory()
{
}

unsigned int PrimitiveFactory::createCube(const comp::Position& position) {
    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, m_material);
    m_ctx.registry.assign<comp::Pipeline>(entity, m_pipeline);
    m_ctx.registry.assign<comp::Position>(entity, position);
    return 0;
}
