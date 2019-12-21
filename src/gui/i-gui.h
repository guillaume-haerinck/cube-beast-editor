#pragma once

enum class GuiEvent {
    APP_LAUNCHED = 0
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
