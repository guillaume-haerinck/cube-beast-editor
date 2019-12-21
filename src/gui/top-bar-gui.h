#pragma once

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class TopBarGui : public IGui {
public:
    TopBarGui(Context& ctx, SingletonComponents& scomps);
    virtual ~TopBarGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
