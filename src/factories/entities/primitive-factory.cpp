#include "primitive-factory.h"

PrimitiveFactory::PrimitiveFactory(Context& context) : m_ctx(context) {
    // TODO move to scomp init in app
    scomp::ConstantBufferIndex cbIndices[] = {
		scomp::PER_FRAME
	};
    m_pipeline = m_ctx.rcommand.createPipeline("res/shaders/basics/basic.vert", "res/shaders/basics/basic.frag", cbIndices, std::size(cbIndices));
}

PrimitiveFactory::~PrimitiveFactory()
{
}

unsigned int PrimitiveFactory::createCube(const comp::Transform& position) {
    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, m_material);
    m_ctx.registry.assign<comp::Pipeline>(entity, m_pipeline);
    m_ctx.registry.assign<comp::Transform>(entity, position);
    return 0;
}
