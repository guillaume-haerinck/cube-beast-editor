#include "history-handler.h"

HistoryHandler::HistoryHandler(SingletonComponents& scomps) : m_scomps(scomps), m_current(0) {}

HistoryHandler::~HistoryHandler() {
    for (IHistory* history : m_history) {
        delete history;
    }
}

void HistoryHandler::pushHistory(IHistory* history) {
    m_history.push_back(history);
    m_current++;
}

bool HistoryHandler::undo() {
    if (m_current > 0) {
        m_history.at(m_current - 1)->undo();
        m_current--;
        return true;
    } else {
        return false;
    }
}

bool HistoryHandler::redo() {
    if (m_current < m_history.size() - 1) {
        m_history.at(m_current - 1)->redo();
        m_current++;
        return true;
    } else {
        return false;
    }
}
