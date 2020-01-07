#pragma once

#include <glm/glm.hpp>

#include "i-gui.h"
#include "context.h"
#include "scomponents/singleton-components.h"

class GenerationGui : public IGui {
public:
    GenerationGui(Context& ctx, SingletonComponents& scomps);
    virtual ~GenerationGui();

    virtual void update() override;
    virtual void onEvent(GuiEvent e) override;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;

    std::vector<glm::ivec3> m_controlPointsXYZ;
    std::vector<float> m_controlPointsWeights;
};
