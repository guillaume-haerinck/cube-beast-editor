#pragma once

#include <met/met.hpp>

#include "core/context.h"
#include "components/physics/position.h"

class CubeFactory {
public:
    CubeFactory(Context& context);
    ~CubeFactory();

    met::entity createCube(const comp::Position& position);

private:
    Context& m_ctx;
};
