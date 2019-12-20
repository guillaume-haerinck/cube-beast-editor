#pragma once

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class ViewportGui : public IGui {
public:
    ViewportGui(Context& ctx, SingletonComponents& scomps);
    virtual ~ViewportGui();

    virtual void update() override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};

