#include "app.h"

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif

#include "systems/render-system.h"
#include "systems/camera-system.h"
#include "systems/selection-system.h"
#include "systems/brushes/voxel-brush-system.h"
#include "gui/viewport-gui.h"
#include "gui/top-bar-gui.h"

bool App::m_instanciated = false;

App::App() : m_running(true), m_ctx(m_scomps) {
	// Ensure that there is only one app
    assert(!m_instanciated && "Application already instanciated !");
	m_instanciated = true;
    
	// Start application
	spdlog::set_pattern("[%l] %^ %v %$");
	initSDL();
    initImgui();

	// Init opengl static states
	glEnable(GL_CULL_FACE);

	// Order GUIs
    m_guis = {
		new TopBarGui(m_ctx, m_scomps),
        new ViewportGui(m_ctx, m_scomps)
    };

	// Send first event
	for (auto gui : m_guis) {
		gui->onEvent(GuiEvent::APP_LAUNCHED);
	}

	// Order system updates
	m_systems = {
		new RenderSystem(m_ctx, m_scomps),
		new SelectionSystem(m_ctx, m_scomps),
		new CameraSystem(m_scomps),
		new VoxelBrushSystem(m_ctx, m_scomps)
	};
}

App::~App() {
    for (IGui* gui : m_guis) {
        delete gui;
    }
	for (ISystem* system : m_systems) {
		delete system;
	}
    m_ctx.rcommand.~RenderCommand();
    ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
    SDL_DestroyWindow(m_window);
	SDL_Quit();
}

/////////////////////////////////////////////////////////////////////////////
////////////////////////////// PUBLIC METHODS ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void App::update() {
	// Feed inputs
	handleSDLEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	// Update our app
	for (ISystem* system : m_systems) {
		system->update();
	}
	
	// Update imgui
    m_ctx.rcommand.unbindVertexBuffer();
	m_ctx.rcommand.unbindRenderTargets();
	for (IGui* gui : m_guis) {
		gui->update();
	}
	
	// Render imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Reset input deltas
	m_scomps.inputs.delta = glm::vec2(0.0f);
	m_scomps.inputs.wheelDelta = 0;

	SDL_GL_SwapWindow(m_window);
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void App::handleSDLEvents() {
	// TEMP
	// FIXME find a better way to handle both one-time event and other which have to last multiple frame until interuption
	m_scomps.inputs.actionState.at(scomp::InputAction::BRUSH_VOX_ADD) = false;
	m_scomps.inputs.actionState.at(scomp::InputAction::BRUSH_VOX_REMOVE) = false;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					break;
	
				default: break;
			}
			continue;	
		}

        switch (e.type) {
        case SDL_QUIT:
            exit();
            break;

        case SDL_MOUSEWHEEL:
            m_scomps.inputs.wheelDelta = e.wheel.y;
            m_scomps.inputs.actionState.at(scomp::InputAction::CAM_DOLLY) = true;
            break;

        case SDL_MOUSEMOTION:
            m_scomps.inputs.delta.x = m_scomps.inputs.mousePos.x - e.button.x + m_scomps.viewportPosTopLeft.x;
            m_scomps.inputs.delta.y = m_scomps.inputs.mousePos.y - e.button.y + m_scomps.viewportPosTopLeft.y;
            m_scomps.inputs.mousePos.x = static_cast<float>(e.button.x - m_scomps.viewportPosTopLeft.x);
            m_scomps.inputs.mousePos.y = static_cast<float>(e.button.y - m_scomps.viewportPosTopLeft.y);
			m_scomps.inputs.NDCMousePos.x = ((float) m_scomps.inputs.mousePos.x / m_scomps.viewportSize.x) * 2.0f - 1.0f;
			m_scomps.inputs.NDCMousePos.y = -(((float) m_scomps.inputs.mousePos.y / m_scomps.viewportSize.y) * 2.0f - 1.0f);
            break;
        

        case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_RIGHT)
            	m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = true;
			else if (e.button.button == SDL_BUTTON_MIDDLE)
				m_scomps.inputs.actionState.at(scomp::InputAction::CAM_PAN) = true;
            break;

        case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_RIGHT)
				m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = false;
			else if (e.button.button == SDL_BUTTON_LEFT)
				m_scomps.inputs.actionState.at(scomp::InputAction::BRUSH_VOX_ADD) = true;
			else if (e.button.button == SDL_BUTTON_MIDDLE)
				m_scomps.inputs.actionState.at(scomp::InputAction::CAM_PAN) = false;
            break;

        default: break;
        }
    }
}

void App::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		spdlog::critical("[SDL2] Unable to initialize SDL: {}", SDL_GetError());
		debug_break();
	}
	SDL_GL_LoadLibrary(NULL);
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(
		"Voxel Editor",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		m_scomps.viewportSize.x, m_scomps.viewportSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
    );
	if (m_window == nullptr) {
        spdlog::critical("[SDL2] Window is null: {}", SDL_GetError());
        debug_break();
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (m_glContext == nullptr) {
        spdlog::critical("[SDL2] OpenGL context is null: {}",  SDL_GetError());
        debug_break();
    }

	SDL_GL_MakeCurrent(m_window, m_glContext);
	SDL_GL_SetSwapInterval(1);

#ifndef __EMSCRIPTEN__
	if (!gladLoadGLES2((GLADloadfunc) SDL_GL_GetProcAddress)) {
		spdlog::critical("[Glad] Glad not init");
		debug_break();
	}
#endif	
}

void App::initImgui() const {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_glContext);
	ImGui_ImplOpenGL3_Init("#version 300 es");
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
