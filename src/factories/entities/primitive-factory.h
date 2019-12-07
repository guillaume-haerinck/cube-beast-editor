#pragma once

#include "context.h"
#include "scomponents/singleton-components.h"
#include "components/physics/transform.h"
#include "components/graphics/material.h"
#include "components/graphics/pipeline.h"

class PrimitiveFactory {
public:
    PrimitiveFactory(Context& context);
    ~PrimitiveFactory();

    unsigned int createCube(const comp::Transform& position);

private:
    Context& m_ctx;
    comp::Material m_material;
    comp::Pipeline m_pipeline;
};
