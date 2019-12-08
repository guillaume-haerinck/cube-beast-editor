#pragma once

#include "i-layer.h"
#include "context.h"

class ViewportLayer : public ILayer {
public:
    ViewportLayer(Context& ctx);
    virtual ~ViewportLayer();

    virtual void update() override;

private:
    Context& m_ctx;
};

