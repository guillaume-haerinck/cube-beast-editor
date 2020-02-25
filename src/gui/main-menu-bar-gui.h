#pragma once

#include <imgui.h>

#include "i-gui.h"
#include "context.h"
#include "data/singleton-components.h"

class MainMenuBarGui : public IGui {
public:
    MainMenuBarGui(Context& ctx, SingletonComponents& scomps);
    virtual ~MainMenuBarGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    void setDefaultLayout();
    void setDefaultStyle();

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;

    ImGuiID m_dockspaceId;
    bool m_setDefaultLayout;
};
