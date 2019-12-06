#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "core/context.h"
#include "systems/i-system.h"

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
    
    std::vector<ISystem*> m_systems;
    Context m_ctx;
};
