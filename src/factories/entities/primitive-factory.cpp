#include "primitive-factory.h"

PrimitiveFactory::PrimitiveFactory(Context& context) : m_ctx(context) {}

PrimitiveFactory::~PrimitiveFactory() {}

met::entity PrimitiveFactory::createCube(const comp::Transform& position) {
    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, m_material);
    m_ctx.registry.assign<comp::Transform>(entity, position);
    return entity;
}
