#pragma once

class IHistory {
public:
    virtual ~IHistory() {};

    virtual void redo() = 0;
    virtual void undo() = 0;
};
