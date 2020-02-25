#pragma once

#include <glm/glm.hpp>

#include "i-gui.h"
#include "context.h"
#include "data/singleton-components.h"

class PaletteGui : public IGui {
public:
    PaletteGui(Context& ctx, SingletonComponents& scomps);
    virtual ~PaletteGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;

    glm::vec3 m_backupColor;
};
