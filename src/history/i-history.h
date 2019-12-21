#pragma once

#include "context.h"
#include "scomponents/singleton-components.h"

class IHistory {
public:
    IHistory();
    virtual ~IHistory();

    virtual void redo() = 0;
    virtual void undo() = 0;

protected:
    Context& m_ctx;
    SingletonComponents& m_scomps;
};
