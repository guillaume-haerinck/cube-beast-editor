#include "app.h"

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "systems/render-system.h"
#include "systems/camera-system.h"
#include "systems/selection-system.h"
#include "systems/brush-system.h"

#include "gui/font-ruda.h"
#include "gui/brush-gui.h"
#include "gui/context-info-bar-gui.h"
#include "gui/generation-gui.h"
#include "gui/main-menu-bar-gui.h"
#include "gui/palette-gui.h"
#include "gui/scene-outline-gui.h"
#include "gui/viewport-gui.h"
#include "gui/viewport-option-bar-gui.h"

bool App::m_instanciated = false;

App::App() : m_running(true), m_ctx(m_scomps) {
	// Ensure that there is only one app
    assert(!m_instanciated && "Application already instanciated !");
	m_instanciated = true;
    
	// Start application
	spdlog::set_pattern("[%l] %^ %v %$");
	initSDL();
    initImgui();

	// Order GUIs
    m_guis = {
		new MainMenuBarGui(m_ctx, m_scomps),
        new ViewportGui(m_ctx, m_scomps),
		new BrushGui(m_ctx, m_scomps),
		new ContextInfoBarGui(m_ctx, m_scomps),
		new GenerationGui(m_ctx, m_scomps),
		new PaletteGui(m_ctx, m_scomps),
		new SceneOutlineGui(m_ctx, m_scomps),
		new ViewportOptionBarGui(m_ctx, m_scomps)
    };

	// Init graphics objects
	m_scomps.constantBuffers.init(m_ctx.rcommand);
	m_scomps.pipelines.init(m_ctx.rcommand, m_scomps.constantBuffers);
	m_scomps.meshes.init(m_ctx.rcommand);
	m_scomps.renderTargets.init(m_ctx.rcommand);

	// Init renderer static states
	m_ctx.rcommand.enableFaceCulling();

	// Order system updates
	m_systems = {
		new RenderSystem(m_ctx, m_scomps),
		new SelectionSystem(m_ctx, m_scomps),
		new CameraSystem(m_scomps),
		new BrushSystem(m_ctx, m_scomps)
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
	ImGui::ShowDemoWindow(); // Temp
	
	// Render imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Reset input deltas
	//m_scomps.inputs.delta = glm::vec2(0.0f);
	//m_scomps.inputs.wheelDelta = 0;

	SDL_GL_SwapWindow(m_window);
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void App::handleSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        switch (e.type) {
        case SDL_QUIT: exit(); break;
		/*

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
			else if (e.button.button == SDL_BUTTON_LEFT)
				m_scomps.isBrushStarted = true;
            break;

        case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_RIGHT)
				m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = false;
			else if (e.button.button == SDL_BUTTON_LEFT)
				m_scomps.isBrushStarted = false;
			else if (e.button.button == SDL_BUTTON_MIDDLE)
				m_scomps.inputs.actionState.at(scomp::InputAction::CAM_PAN) = false;
            break;
			*/

        default: break;
        }

/*
		if (!m_scomps.isViewportHovered)
			m_scomps.inputs.actionState.fill(false);
			*/
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
/*
	m_window = SDL_CreateWindow(
		"Voxel Editor",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		m_scomps.viewportSize.x, m_scomps.viewportSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
    );
	*/
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

	// Set icon
	Uint16 iconData[] = {
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
    0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
    0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
    0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
    0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
    0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
    0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
    0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
    0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
    0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
    0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
    0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
    0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
    0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
    0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
    0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
    0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
    0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
    0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
    0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
    0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
    0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
  };

  SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(iconData, 16, 16, 16, 16*2, 0x0f00, 0x00f0, 0x000f, 0xf000);
  SDL_SetWindowIcon(m_window, surface);
  SDL_FreeSurface(surface);

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
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(compressed_data_base85, 15.0f);
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
