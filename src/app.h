#pragma once

#include <SDL2/SDL.h>

#include "context.h"
#include "scomponents/singleton-components.h"
#include "gui/i-gui.h"
#include "systems/i-system.h"

/**
 * @brief Base root of the app
 */
class App {
public:
    App();
    ~App();

    void update();

    bool isRunning() const;
    void exit();

private:
    void initSDL();
    void initImgui() const;
    void handleSDLEvents();

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    static bool m_instanciated;
    bool m_running;
    
    Context m_ctx;
    SingletonComponents m_scomps;
    std::vector<IGui*> m_guis;
    std::vector<ISystem*> m_systems;
};
