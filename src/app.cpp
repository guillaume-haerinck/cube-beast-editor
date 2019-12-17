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
#include "graphics/primitive-data.h"
#include "graphics/constant-buffer.h"
#include "scomponents/graphics/render-targets.h"

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

	// Init opengl static states
	glEnable(GL_CULL_FACE);

	// Order system updates
	m_systems = {
		new RenderSystem(m_ctx, m_scomps),
		new SelectionSystem(m_ctx, m_scomps),
		new CameraSystem(m_scomps),
		new VoxelBrushSystem(m_ctx, m_scomps)
	};

    // Order GUIs
    m_guis = {
        new ViewportGui(m_ctx)
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
        switch (e.type) {
        case SDL_QUIT:
            exit();
            break;

        case SDL_MOUSEWHEEL:
            m_scomps.inputs.wheelDelta = e.wheel.y;
            m_scomps.inputs.actionState.at(scomp::InputAction::CAM_DOLLY) = true;
            break;

        case SDL_MOUSEMOTION:
            m_scomps.inputs.delta.x = m_scomps.inputs.mousePos.x - e.button.x;
            m_scomps.inputs.delta.y = m_scomps.inputs.mousePos.y - e.button.y;
            m_scomps.inputs.mousePos.x = static_cast<float>(e.button.x);
            m_scomps.inputs.mousePos.y = static_cast<float>(e.button.y);
			m_scomps.inputs.NDCMousePos.x = (e.button.x / 500.0f) * 2.0f - 1.0f; // TODO get screensize from scomp
			m_scomps.inputs.NDCMousePos.y = -((e.button.y / 500.0f) * 2.0f - 1.0f);
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

        default:
            break;
        }
    }

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
        m_scomps.inputs.actionState.fill(false);
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
		m_ctx.rcommand.createConstantBuffer(scomp::ConstantBufferIndex::PER_NI_MESH, sizeof(cb::perNiMesh));
		m_ctx.rcommand.createConstantBuffer(scomp::ConstantBufferIndex::PER_FRAME, sizeof(cb::perFrame));

		// TODO use arrays
		m_ctx.rcommand.createConstantBuffer(scomp::ConstantBufferIndex::PER_MATERIAL_CHANGE, sizeof(cb::perMaterialChange));
		m_ctx.rcommand.createConstantBuffer(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE, sizeof(cb::perLightChange));
	}

    // Init pipelines
    {
		std::vector<scomp::ConstantBufferIndex> cbIndices = {
            scomp::ConstantBufferIndex::PER_FRAME
        };

		// Geometry
		const char* VSGeo = 
			#include "graphics/shaders/geometry.vert"
		;
		const char* FSGeo =
			#include "graphics/shaders/geometry.frag"
		;
        m_ctx.rcommand.createPipeline(scomp::PipelineIndex::PIP_GEOMETRY, VSGeo, FSGeo, cbIndices);

		// Grid
		const char* VSGrid = 
			#include "graphics/shaders/grid.vert"
		;
		const char* FSGrid =
			#include "graphics/shaders/grid.frag"
		;
        m_ctx.rcommand.createPipeline(scomp::PipelineIndex::PIP_GRID, VSGrid, FSGrid, cbIndices);

		// Debug draw
		const char* VSDdraw = 
			#include "graphics/shaders/ddraw.vert"
		;
		const char* FSDdraw =
			#include "graphics/shaders/ddraw.frag"
		;
        m_ctx.rcommand.createPipeline(scomp::PipelineIndex::PIP_DDRAW, VSDdraw, FSDdraw, cbIndices);

		// Lighting
		cbIndices = {
			scomp::ConstantBufferIndex::PER_FRAME,
			scomp::ConstantBufferIndex::PER_LIGHT_CHANGE
		};
		const char* VSLighting = 
			#include "graphics/shaders/lighting.vert"
		;
		const char* FSLighting =
			#include "graphics/shaders/lighting.frag"
		;
        m_ctx.rcommand.createPipeline(scomp::PipelineIndex::PIP_LIGHTING, VSLighting, FSLighting, cbIndices);

		// Gui
		cbIndices = {
			 scomp::ConstantBufferIndex::PER_FRAME,
			 scomp::ConstantBufferIndex::PER_NI_MESH
		};
		const char* VSGui = 
			#include "graphics/shaders/gui.vert"
		;
		const char* FSGui =
			#include "graphics/shaders/gui.frag"
		;
        m_ctx.rcommand.createPipeline(scomp::PipelineIndex::PIP_GUI, VSGui, FSGui, cbIndices);
    }

	// Update light constant buffer
	{
		scomp::ConstantBuffer& perLightChangeCB = m_scomps.constantBuffers.at(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE);
		cb::perLightChange cbData;
		cbData.color = glm::vec3(1, 0, 0);

		m_ctx.rcommand.updateConstantBuffer(perLightChangeCB, &cbData);
	}

    // Init Render Targets
    {
		// FIMXE on linux second color render target cannot be float or framebuffer incomplete
        PipelineOutputDescription outputDescription = {
			{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
			{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::UCHAR, RenderTargetChannels::RGB, "Geometry_Normal" },
			{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::UCHAR, RenderTargetChannels::RGB, "Geometry_WorldPosition" },
            { RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor" },
			{ RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
        };
        m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_GEOMETRY, outputDescription);
    }

	// Init dynamic debug draw vertex buffer
	{
		// Attributes
		glm::vec3 positions[] = {
        	glm::vec3(10, 10, 10), glm::vec3(0, 0, 0),
			glm::vec3(10, 10, 10), glm::vec3(0, 0, 0),
			glm::vec3(10, 10, 10), glm::vec3(0, 0, 0),
			glm::vec3(10, 10, 10), glm::vec3(0, 0, 0),
			glm::vec3(10, 10, 10), glm::vec3(0, 0, 0)
		};
		scomp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(positions, static_cast<unsigned int>(std::size(positions)), sizeof(glm::vec3), scomp::AttributeBufferUsage::DYNAMIC_DRAW, scomp::AttributeBufferType::PER_VERTEX_ANY);
		
		// Vertex buffer
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" }
		};
		scomp::AttributeBuffer attributeBuffers[] = {
			positionBuffer
		};
		m_scomps.ddrawVb = m_ctx.rcommand.createVertexBuffer(inputDescription, attributeBuffers);
	}

	// Init Plane Mesh
	{
		// Attributes
		scomp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(&squareData::positions, static_cast<unsigned int>(std::size(squareData::positions)), sizeof(glm::vec3));
		scomp::AttributeBuffer texCoordBuffer = m_ctx.rcommand.createAttributeBuffer(&squareData::texCoords, static_cast<unsigned int>(std::size(squareData::texCoords)), sizeof(glm::vec2));

		// Vertex & Index buffers
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TexCoord" }
		};
		scomp::AttributeBuffer attributeBuffers[] = {
			positionBuffer, texCoordBuffer
		};
		scomp::VertexBuffer vb = m_ctx.rcommand.createVertexBuffer(inputDescription, attributeBuffers);
		scomp::IndexBuffer ib = m_ctx.rcommand.createIndexBuffer(squareData::indices, static_cast<unsigned int>(std::size(squareData::indices)), scomp::IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		scomp::Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_scomps.planeMesh = mesh;
	}

	// Init CubeMesh
	{
		// Attributes
		scomp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		scomp::AttributeBuffer normalBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::normals, static_cast<unsigned int>(std::size(cubeData::normals)), sizeof(glm::vec3));
		std::array<glm::vec3, 15> translations; // TODO set to scene size
		scomp::AttributeBuffer translationInstanceBuffer = m_ctx.rcommand.createAttributeBuffer(translations.data(), static_cast<unsigned int>(translations.size()), sizeof(glm::vec3), scomp::AttributeBufferUsage::DYNAMIC_DRAW, scomp::AttributeBufferType::PER_INSTANCE_TRANSLATION);
		std::array<glm::vec3, 15> entityIds;
		scomp::AttributeBuffer entityInstanceBuffer = m_ctx.rcommand.createAttributeBuffer(entityIds.data(), static_cast<unsigned int>(entityIds.size()), sizeof(glm::vec3), scomp::AttributeBufferUsage::DYNAMIC_DRAW, scomp::AttributeBufferType::PER_INSTANCE_ENTITY_ID);

		// Vertex & Index buffers
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float3, "Normal" },
			{ ShaderDataType::Float3, "Translation", BufferElementUsage::PerInstance },
			{ ShaderDataType::Float3, "EntityId", BufferElementUsage::PerInstance }
		};
		scomp::AttributeBuffer attributeBuffers[] = {
			positionBuffer, normalBuffer, translationInstanceBuffer, entityInstanceBuffer
		};
		scomp::VertexBuffer vb = m_ctx.rcommand.createVertexBuffer(inputDescription, attributeBuffers);
		scomp::IndexBuffer ib = m_ctx.rcommand.createIndexBuffer(cubeData::indices, static_cast<unsigned int>(std::size(cubeData::indices)), scomp::IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		scomp::Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_scomps.cubeMesh = mesh;
	}

	// Init Inverted CubeMesh
	{
		// Attributes
		scomp::AttributeBuffer positionBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		scomp::AttributeBuffer texCoordBuffer = m_ctx.rcommand.createAttributeBuffer(&cubeData::texCoords, static_cast<unsigned int>(std::size(cubeData::texCoords)), sizeof(glm::vec2));

		// Vertex & Index buffers
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TexCoord" }
		};
		scomp::AttributeBuffer attributeBuffers[] = {
			positionBuffer, texCoordBuffer
		};
		scomp::VertexBuffer vb = m_ctx.rcommand.createVertexBuffer(inputDescription, attributeBuffers);
		scomp::IndexBuffer ib = m_ctx.rcommand.createIndexBuffer(cubeData::invertIndices, static_cast<unsigned int>(std::size(cubeData::invertIndices)), scomp::IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		scomp::Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_scomps.invertCubeMesh = mesh;
	}
}

/////////////////////////////////////////////////////////////////////////////
//////////////////////////// GETTERS & SETTERS //////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool App::isRunning() const { return m_running; }
void App::exit() { m_running = false; }
