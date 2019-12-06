#pragma once

#include <met/met.hpp>

#include "core/context.h"

class CubeFactory {
public:
    CubeFactory(Context& context);
    ~CubeFactory();

    met::entity createCube();

private:
    Context& m_ctx;
};
