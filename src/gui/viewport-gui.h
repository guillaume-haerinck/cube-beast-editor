#pragma once

#include "i-gui.h"
#include "context.h"

class ViewportGui : public IGui {
public:
    ViewportGui(Context& ctx);
    virtual ~ViewportGui();

    virtual void update() override;

private:
    Context& m_ctx;
};

