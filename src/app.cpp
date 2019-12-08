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
#include "layers/viewport-layer.h"
#include "graphics/primitive-data.h"
#include "graphics/constant-buffer.h"

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
		new RenderSystem(m_ctx, m_scomps),
		new CameraSystem(m_scomps)
	};

    // Order GUIs
    m_layers = {
        new ViewportLayer(m_ctx)
    };
}

App::~App() {
    for (ILayer* layer : m_layers) {
        delete layer;
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
	m_ctx.rcommand.clear();

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
	for (ILayer* layer : m_layers) {
		layer->update();
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
	// Init Constant Buffers
	{
		m_ctx.rcommand.createConstantBuffer(scomp::ConstantBufferIndex::PER_FRAME, sizeof(cb::perFrame));
	}

    // Init Render Targets
    {
        PipelineOutputDescription outputDescription = {
            { RenderTargetUsage::Color, RenderTargetType::Texture, "NormalColor" }
        };
        scomp::RenderTargets rts = m_ctx.rcommand.createRenderTargets(outputDescription);
        m_scomps.renderTargets.at(scomp::RenderTargetsType::RTT_PICKING) = rts;
    }

	// Init CubeMesh
	{
		// Layout
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float3, "Normal" },
			{ ShaderDataType::Float3, "Translation", BufferElementUsage::PerInstance }
		};
		
		// Attributes
		scomp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		scomp::AttributeBuffer normalBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		std::array< comp::Transform, 15> translations; // TODO set to scene size
		scomp::AttributeBuffer translationInstanceBuffer = m_ctx.rcommand.createAttributeBuffer(translations.data(), static_cast<unsigned int>(translations.size()), sizeof(glm::vec3), scomp::AttributeBufferUsage::DYNAMIC_DRAW, scomp::AttributeBufferType::PER_INSTANCE_POSITION);

		// Vertex & Index buffers
		scomp::AttributeBuffer attributeBuffers[] = {
			positionBuffer, normalBuffer, translationInstanceBuffer
		};
		scomp::VertexBuffer vb = m_ctx.rcommand.createVertexBuffer(inputDescription, attributeBuffers);
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
			m_scomps.inputs.wheelDelta = e.wheel.y;
			m_scomps.inputs.actionState.at(scomp::InputAction::CAM_DOLLY) = true;
			break;

		case SDL_MOUSEMOTION: {
			int newPosX = e.button.x;
			int newPosY = e.button.y;
			m_scomps.inputs.delta.x = m_scomps.inputs.mousePos.x - newPosX;
			m_scomps.inputs.delta.y = m_scomps.inputs.mousePos.y - newPosY;
			m_scomps.inputs.mousePos.x = static_cast<float>(newPosX);
			m_scomps.inputs.mousePos.y = static_cast<float>(newPosY);
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
			m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = true;
			break;

		case SDL_MOUSEBUTTONUP:
			m_scomps.inputs.actionState.at(scomp::InputAction::CAM_ORBIT) = false;
			break;

		default:
			break;
		}
	}

	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		m_scomps.inputs.actionState.fill(false);
	}
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

SDL_Window* App::getWindow() const { return m_window; }
bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
