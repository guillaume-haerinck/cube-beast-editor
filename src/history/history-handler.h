#pragma once

#include <deque>

#include "i-history.h"
#include "data/singleton-components.h"

class HistoryHandler {
public:
    HistoryHandler(SingletonComponents& scomps);
    ~HistoryHandler();

    void pushHistory(IHistory* history);
    bool undo();
    bool redo();

    size_t size() const { return m_history.size(); }

private:
    SingletonComponents& m_scomps;
    std::deque<IHistory*> m_history;
    unsigned int m_current;
};
