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

bool App::m_instanciated = false;

App::App() : m_running(true)
{
    assert(!m_instanciated);
	m_instanciated = true;

    spdlog::set_pattern("[%l] %^ %v %$");

	initSDL();
    initImgui();
    m_ctx.rcommand = std::make_unique<RenderCommand>(m_ctx);

	m_systems = {
		new RenderSystem(m_ctx)
	};

	glEnable(GL_DEPTH_TEST);
}

App::~App() {
	for (ISystem* system : m_systems) {
		delete system;
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	glBindVertexArray(0);
	renderMenu();

	// Render imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_window);
}

/////////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////


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

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(
		"OpenGL Playground",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		500, 500,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
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
}

void App::handleSDLEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch (e.type) {
		case SDL_QUIT:
			exit();
			break;

		case SDL_MOUSEWHEEL:
			break;

		case SDL_MOUSEMOTION:
			break;

		case SDL_MOUSEBUTTONDOWN:
			break;

		case SDL_MOUSEBUTTONUP:
			break;

		default:
			break;
		}
	}

	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
	}
}


void App::renderMenu() {
	ImGui::Begin("Main debug window");

	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::CollapsingHeader("Help")) {
		ImGui::Text("Camera controls :");
		ImGui::BulletText("Orbit - Left mouse button / Middle mouse button");
		ImGui::BulletText("Pan - Right mouse button");
		ImGui::BulletText("Zoom - Mousewheel");
		ImGui::BulletText("Reset - Left mouse double click");
	}

	ImGui::Spacing();

	ImGui::Text("Exemples:");

	ImGui::End();
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

SDL_Window* App::getWindow() const { return m_window; }
bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
