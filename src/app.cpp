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
#include "graphics/primitive-data.h"

#include "factories/entities/primitive-factory.h"
#include "components/physics/position.h"

bool App::m_instanciated = false;

App::App() : m_running(true), m_ctx(m_scomps) {
	// Ensure that there is only one app
    assert(!m_instanciated && "Application already instanciated !");
	m_instanciated = true;
    
	// Start application
	spdlog::set_pattern("[%l] %^ %v %$");
	initSDL();
    initImgui();
	initSingletonComponents();
	glEnable(GL_DEPTH_TEST);

	// Order system updates
	m_systems = {
		new RenderSystem(m_ctx, m_scomps)
	};

	// TEMP
	PrimitiveFactory factory(m_ctx);
	factory.createCube({0, 0, 0});
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
	for (ILayer* layer : m_layers) {
		layer->update();
	}
	
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
		"Voxel Editor",
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

void App::initSingletonComponents() {
	// Init CubeMesh
	{
		// Layout
		VertexInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float3, "Normal" },
			{ ShaderDataType::Float3, "Translation", BufferElementUsage::PerInstance }
		};
		
		// Shared data
		scomp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		scomp::AttributeBuffer normalBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		
		// Instance data
		// TODO set to scene size
		std::array<glm::vec3, 15> translations;
		translations.fill(glm::vec3(1));
		// TEMP
		translations.at(1) = glm::vec3(0);
		translations.at(2) = glm::vec3(-1);
		scomp::AttributeBuffer translationInstanceBuffer = m_ctx.rcommand.createAttributeBuffer(translations.data(), static_cast<unsigned int>(translations.size()), sizeof(glm::vec3), scomp::AttributeBufferUsage::DYNAMIC_DRAW, scomp::AttributeBufferType::PER_INSTANCE_POSITION);

		// Vertex buffer
		scomp::AttributeBuffer attributeBuffers[] = {
			positionBuffer, normalBuffer, translationInstanceBuffer
		};
		scomp::VertexBuffer vb = m_ctx.rcommand.createVertexBuffer(inputDescription, attributeBuffers);

		// Index buffer
		scomp::IndexBuffer ib = m_ctx.rcommand.createIndexBuffer(cubeData::indices, static_cast<unsigned int>(std::size(cubeData::indices)), scomp::IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		scomp::Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_scomps.cubeMesh = mesh;
	}
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

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

SDL_Window* App::getWindow() const { return m_window; }
bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
