#include "viewport-gui.h"

#include <met/met.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "components/graphics/material.h"
#include "components/physics/transform.h"
#include "graphics/primitive-data.h"
#include "graphics/constant-buffer.h"
#include "scomponents/graphics/render-targets.h"

// Temp
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif

ViewportGui::ViewportGui(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {
    comp::Material material;

    met::entity entity = m_ctx.registry.create();
    m_ctx.registry.assign<comp::Material>(entity, material);
    m_ctx.registry.assign<comp::Transform>(entity, glm::ivec3(1, 0, 0));
}

ViewportGui::~ViewportGui() {}

void ViewportGui::update() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Viewport", 0, window_flags);
    {
		ImGui::PopStyleVar(1);

		if (ImGui::IsWindowHovered()) {
			//m_scomps.isViewportHovered = true;
		} else {
			//m_scomps.isViewportHovered = false;
		}

        // Handle framebuffer
        ImVec2 viewportSize = ImGui::GetWindowSize();
		/*
        if (viewportSize.x != m_scomps.viewportSize.x || viewportSize.y != m_scomps.viewportSize.y) {
            m_scomps.viewportSize = glm::ivec2(viewportSize.x, viewportSize.y);
            glViewport(0, 0, m_scomps.viewportSize.x, m_scomps.viewportSize.y);
            m_scomps.camera.m_proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), (float) m_scomps.viewportSize.x, (float) m_scomps.viewportSize.y, 0.1f, 100.0f);
            // TODO abstract
			// RemakeFramebuffers
            {
                //glDeleteFramebuffers(1, &m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY).frameBufferId);
                PipelineOutputDescription outputDescription = {
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" },
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_WorldPosition" },
                    { RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor" },
                    { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
                };
                //m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_GEOMETRY, outputDescription);

                //glDeleteFramebuffers(1, &m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_FINAL).frameBufferId);
                outputDescription = {
                    { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Color" },
                    { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
                };
                //m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_FINAL, outputDescription);
            }
        }

        // Update viewport position & draw framebuffer
        {
            const ImVec2 viewportPosTopLeft = ImGui::GetCursorScreenPos();
            m_scomps.viewportPosTopLeft = glm::ivec2(viewportPosTopLeft.x, viewportPosTopLeft.y);
			/*
			ImGui::GetWindowDrawList()->AddImage(
				(void*) m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_FINAL).textureIds.at(0),
				viewportPosTopLeft,
				ImVec2(viewportPosTopLeft.x + m_scomps.viewportSize.x, viewportPosTopLeft.y + m_scomps.viewportSize.y),
				ImVec2(0, 1), ImVec2(1, 0)
			);
        }
		*/
    }
    ImGui::End();
}

void ViewportGui::onEvent(GuiEvent e) {
    switch (e) {
        default: break;
    }
}

/*
void ViewportGui::initGraphicsSingletonComponents() {



    // Init pipelines
    {
		
    }

	// Update light constant buffer
	{
		scomp::ConstantBuffer& perLightChangeCB = m_scomps.constantBuffers.at(static_cast<unsigned int>(scomp::ConstantBufferIndex::PER_LIGHT_CHANGE));
		cb::perLightChange cbData;
		cbData.color = glm::vec3(0.5, 0.5, 0.5);

		m_ctx.rcommand.updateConstantBuffer(perLightChangeCB, &cbData);
	}

    // Init Render Targets
    {
        PipelineOutputDescription outputDescription = {
			{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
			{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" },
			{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_WorldPosition" },
            { RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor" },
			{ RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
        };
        m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_GEOMETRY, outputDescription);

        outputDescription = {
            { RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Color" },
            { RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
        };
        m_ctx.rcommand.createRenderTargets(scomp::RenderTargetsIndex::RTT_FINAL, outputDescription);
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
*/
