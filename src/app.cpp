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
#include "gui/font-awesome.h"
#include "gui/icons-awesome.h"
#include "gui/icons.h"
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
	m_scomps.renderTargets.init(m_ctx.rcommand, m_scomps.viewport);
	m_scomps.textures.init(m_ctx.rcommand);

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

	m_scomps.constantBuffers.destroy(m_ctx.rcommand);
	m_scomps.pipelines.destroy(m_ctx.rcommand);
	m_scomps.meshes.destroy(m_ctx.rcommand);
	m_scomps.renderTargets.destroy(m_ctx.rcommand);
	m_scomps.textures.destroy(m_ctx.rcommand);
	
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
	m_scomps.inputs.m_posDelta = glm::vec2(0.0f);
	m_scomps.inputs.m_wheelDelta = 0;

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

        case SDL_MOUSEWHEEL:
            m_scomps.inputs.m_wheelDelta = e.wheel.y;
            m_scomps.inputs.m_actionState.at(static_cast<unsigned int>(InputAction::CAM_DOLLY)) = true;
            break;

        case SDL_MOUSEMOTION:
            m_scomps.inputs.m_posDelta.x = m_scomps.inputs.mousePos().x - e.button.x + m_scomps.viewport.posTopLeft().x;
            m_scomps.inputs.m_posDelta.y = m_scomps.inputs.mousePos().y - e.button.y + m_scomps.viewport.posTopLeft().y;
            m_scomps.inputs.m_mousePos.x = static_cast<float>(e.button.x - m_scomps.viewport.posTopLeft().x);
            m_scomps.inputs.m_mousePos.y = static_cast<float>(e.button.y - m_scomps.viewport.posTopLeft().y);
			m_scomps.inputs.m_ndcMousePos.x = ((float) m_scomps.inputs.mousePos().x / m_scomps.viewport.size().x) * 2.0f - 1.0f;
			m_scomps.inputs.m_ndcMousePos.y = -(((float) m_scomps.inputs.mousePos().y / m_scomps.viewport.size().y) * 2.0f - 1.0f);
            break;
        
        case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_RIGHT)
				m_scomps.inputs.m_actionState.at(static_cast<unsigned int>(InputAction::CAM_ORBIT)) = true;
			else if (e.button.button == SDL_BUTTON_MIDDLE)
				m_scomps.inputs.m_actionState.at(static_cast<unsigned int>(InputAction::CAM_PAN)) = true;
			else if (e.button.button == SDL_BUTTON_LEFT)
				m_scomps.brush.m_started = true;
            break;

        case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_RIGHT)
				m_scomps.inputs.m_actionState.at(static_cast<unsigned int>(InputAction::CAM_ORBIT)) = false;
			else if (e.button.button == SDL_BUTTON_LEFT)
				m_scomps.brush.m_started = false;
			else if (e.button.button == SDL_BUTTON_MIDDLE)
				m_scomps.inputs.m_actionState.at(static_cast<unsigned int>(InputAction::CAM_PAN)) = false;
            break;

        default: break;
        }

		if (!m_scomps.viewport.isHovered())
			m_scomps.inputs.m_actionState.fill(false);
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
		m_scomps.viewport.size().x, m_scomps.viewport.size().y,
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

	// Set icon
	//SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(iconsData::sdl, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);
	//SDL_SetWindowIcon(m_window, surface);
	//SDL_FreeSurface(surface);

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

	// Custom Fonts
	io.Fonts->AddFontFromMemoryCompressedTTF(ruda_compressed_data, ruda_compressed_size, 16.0f);
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromMemoryCompressedTTF(awesome_compressed_data, awesome_compressed_size, 16.0f, &icons_config, icons_ranges);
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
