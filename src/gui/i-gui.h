#pragma once

enum class GuiEvent {
    TEST = 0
};

/**
 * @brief Graphical user interfaces
 */
class IGui {
public:
    virtual ~IGui() {}
    virtual void update() = 0;
    virtual void onEvent(GuiEvent e) = 0;
};
