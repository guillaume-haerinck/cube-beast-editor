#pragma once

#include "systems/i-system.h"
#include "context.h"

class VoxelBrushSystem : public ISystem {
public:
    VoxelBrushSystem(Context& ctx, SingletonComponents& scomps);
    virtual ~VoxelBrushSystem();

	void update() override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
