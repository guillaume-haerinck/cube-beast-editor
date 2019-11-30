#pragma once

#include <SDL2/SDL.h>
#include <memory>

/**
 * @brief Base root of the app
 */
class App {
public:
    App();
    ~App();

    void update();

public:
    SDL_Window* getWindow() const;
    bool isRunning() const;
    void exit();

private:
    void initSDL();
    void initImgui() const;

    void handleSDLEvents();
    void renderMenu();

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    static bool m_instanciated;
    bool m_running;
};
