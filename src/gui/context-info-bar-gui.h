#pragma once

#include "i-gui.h"
#include "context.h"
#include "data/singleton-components.h"

class ContextInfoBarGui : public IGui {
public:
    ContextInfoBarGui(Context& ctx, SingletonComponents& scomps);
    virtual ~ContextInfoBarGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
