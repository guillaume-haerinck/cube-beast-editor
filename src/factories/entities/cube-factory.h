#pragma once

#include <met/met.hpp>

#include "context.h"
#include "scomponents/singleton-components.h"
#include "components/physics/position.h"
#include "components/graphics/material.h"
#include "components/graphics/pipeline.h"

class CubeFactory {
public:
    CubeFactory(Context& context);
    ~CubeFactory();

    met::entity createCube(const comp::Position& position);

private:
    Context& m_ctx;
    comp::Material m_material;
    comp::Pipeline m_pipeline;
};
