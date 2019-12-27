#pragma once

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class PaletteGui : public IGui {
public:
    PaletteGui(Context& ctx, SingletonComponents& scomps);
    virtual ~PaletteGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
