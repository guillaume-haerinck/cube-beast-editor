#pragma once

#include "i-gui.h"
#include "context.h"
#include "data/singleton-components.h"

class ViewportOptionBarGui : public IGui {
public:
    ViewportOptionBarGui(Context& ctx, SingletonComponents& scomps);
    virtual ~ViewportOptionBarGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    bool drawButton(const char* text, const char* tooltip);

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
