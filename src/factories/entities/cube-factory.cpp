#include "cube-factory.h"

#include "components/graphics/material.h"
#include "components/graphics/pipeline.h"

CubeFactory::CubeFactory(Context& context) : m_ctx(context) {}

CubeFactory::~CubeFactory()
{
}

met::entity CubeFactory::createCube(comp::Position position) {
	comp::Pipeline pipeline = m_ctx.rcommand.createPipeline("res/shaders/basics/basic.vert", "res/shaders/basics/basic.frag");
    comp::Material material;
    
    // Assign data to entity
    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, material);
    m_ctx.registry.assign<comp::Pipeline>(entity, pipeline);
    m_ctx.registry.assign<comp::Position>(entity, position);
    return entity;
}
