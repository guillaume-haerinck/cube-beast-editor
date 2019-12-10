#pragma once

/**
 * @brief Graphical user interfaces
 */
class IGui {
public:
    virtual ~IGui() {}
    virtual void update() = 0;
};
