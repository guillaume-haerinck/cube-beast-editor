#pragma once

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class ViewportOptionBarGui : public IGui {
public:
    ViewportOptionBarGui(Context& ctx, SingletonComponents& scomps);
    virtual ~ViewportOptionBarGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
