#pragma once

/**
 * @brief Graphical user interfaces
 */
class ILayer {
public:
    virtual ~ILayer() {}
    virtual void update() = 0;
};
