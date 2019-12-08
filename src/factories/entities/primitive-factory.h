#pragma once

#include <met/met.hpp>

#include "context.h"
#include "components/physics/transform.h"
#include "components/graphics/material.h"

class PrimitiveFactory {
public:
    PrimitiveFactory(Context& context);
    ~PrimitiveFactory();

    met::entity createCube(const comp::Transform& position);

private:
    Context& m_ctx;
    comp::Material m_material;
};
