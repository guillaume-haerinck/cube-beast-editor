#include "primitive-factory.h"

PrimitiveFactory::PrimitiveFactory(Context& context) : m_ctx(context) {
    scomp::ConstantBufferIndex cbIndices[] = {
		scomp::ConstantBufferIndex::PER_FRAME
	};
    m_pipeline = m_ctx.rcommand.createPipeline("res/shaders/basics/basic.vert", "res/shaders/basics/basic.frag", cbIndices, std::size(cbIndices));
}

PrimitiveFactory::~PrimitiveFactory()
{
}

met::entity PrimitiveFactory::createCube(const comp::Transform& position) {
    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, m_material);
    m_ctx.registry.assign<comp::Pipeline>(entity, m_pipeline);
    m_ctx.registry.assign<comp::Transform>(entity, position);
    return entity;
}
