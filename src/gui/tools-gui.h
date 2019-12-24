#pragma once

#include <imgui.h>

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class ToolsGui : public IGui {
public:
    ToolsGui(Context& ctx, SingletonComponents& scomps);
    virtual ~ToolsGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
