#pragma once

#include <met/met.hpp>

#include "context.h"
#include "scomponents/singleton-components.h"
#include "components/physics/position.h"

class CubeFactory {
public:
    CubeFactory(Context& context);
    ~CubeFactory();

    met::entity createCube(comp::Position position);

private:
    Context& m_ctx;
};
