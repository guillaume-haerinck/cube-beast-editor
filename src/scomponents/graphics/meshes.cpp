#include "meshes.h"

#include "graphics/render-command.h"
#include "graphics/primitive-data.h"

void Meshes::init(RenderCommand& rcommand) {
    // Init Plane Mesh
	{
		// Attributes
		AttributeBuffer positionBuffer = rcommand.createAttributeBuffer(&squareData::positions, static_cast<unsigned int>(std::size(squareData::positions)), sizeof(glm::vec3));
		AttributeBuffer texCoordBuffer = rcommand.createAttributeBuffer(&squareData::texCoords, static_cast<unsigned int>(std::size(squareData::texCoords)), sizeof(glm::vec2));

		// Vertex & Index buffers
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TexCoord" }
		};
		AttributeBuffer attributeBuffers[] = {
			positionBuffer, texCoordBuffer
		};
		VertexBuffer vb = rcommand.createVertexBuffer(inputDescription, attributeBuffers);
		IndexBuffer ib = rcommand.createIndexBuffer(squareData::indices, static_cast<unsigned int>(std::size(squareData::indices)), IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_plane = mesh;
	}

	// Init CubeMesh
	{
		// Attributes
		AttributeBuffer positionBuffer = rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		AttributeBuffer normalBuffer = rcommand.createAttributeBuffer(&cubeData::normals, static_cast<unsigned int>(std::size(cubeData::normals)), sizeof(glm::vec3));
		std::array<glm::vec3, 15> translations; // TODO set to scene size
		AttributeBuffer translationInstanceBuffer = rcommand.createAttributeBuffer(translations.data(), static_cast<unsigned int>(translations.size()), sizeof(glm::vec3), AttributeBufferUsage::DYNAMIC_DRAW, AttributeBufferType::PER_INSTANCE_TRANSLATION);
		std::array<glm::vec3, 15> entityIds;
		AttributeBuffer entityInstanceBuffer = rcommand.createAttributeBuffer(entityIds.data(), static_cast<unsigned int>(entityIds.size()), sizeof(glm::vec3), AttributeBufferUsage::DYNAMIC_DRAW, AttributeBufferType::PER_INSTANCE_ENTITY_ID);

		// Vertex & Index buffers
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float3, "Normal" },
			{ ShaderDataType::Float3, "Translation", BufferElementUsage::PerInstance },
			{ ShaderDataType::Float3, "EntityId", BufferElementUsage::PerInstance }
		};
		AttributeBuffer attributeBuffers[] = {
			positionBuffer, normalBuffer, translationInstanceBuffer, entityInstanceBuffer
		};
		VertexBuffer vb = rcommand.createVertexBuffer(inputDescription, attributeBuffers);
		IndexBuffer ib = rcommand.createIndexBuffer(cubeData::indices, static_cast<unsigned int>(std::size(cubeData::indices)), IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_cube = mesh;
	}

	// Init Inverted CubeMesh
	{
		// Attributes
		AttributeBuffer positionBuffer = rcommand.createAttributeBuffer(&cubeData::positions, static_cast<unsigned int>(std::size(cubeData::positions)), sizeof(glm::vec3));
		AttributeBuffer texCoordBuffer = rcommand.createAttributeBuffer(&cubeData::texCoords, static_cast<unsigned int>(std::size(cubeData::texCoords)), sizeof(glm::vec2));

		// Vertex & Index buffers
		PipelineInputDescription inputDescription = {
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TexCoord" }
		};
		AttributeBuffer attributeBuffers[] = {
			positionBuffer, texCoordBuffer
		};
		VertexBuffer vb = rcommand.createVertexBuffer(inputDescription, attributeBuffers);
		IndexBuffer ib = rcommand.createIndexBuffer(cubeData::invertIndices, static_cast<unsigned int>(std::size(cubeData::invertIndices)), IndexBuffer::dataType::UNSIGNED_BYTE);

		// Save data
		Mesh mesh;
		mesh.ib = ib;
		mesh.vb = vb;
		m_invertCube = mesh;
	}
}
