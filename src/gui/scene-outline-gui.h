#pragma once

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class SceneOutlineGui : public IGui {
public:
    SceneOutlineGui(Context& ctx, SingletonComponents& scomps);
    virtual ~SceneOutlineGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
