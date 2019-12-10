#pragma once

#include "i-system.h"
#include "context.h"

class SelectionSystem : public ISystem {
public:
    SelectionSystem(Context& ctx, SingletonComponents& scomps);
    virtual ~SelectionSystem();

	void update() override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
    met::entity lastSelect;
};
