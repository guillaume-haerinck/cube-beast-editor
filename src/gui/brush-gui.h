#pragma once

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class BrushGui : public IGui {
public:
    BrushGui(Context& ctx, SingletonComponents& scomps);
    virtual ~BrushGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    bool drawButton(const char* text, bool toggled = false);

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
