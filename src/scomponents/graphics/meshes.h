#pragma once

#include <vector>
#include <string>

#include "graphics/pipeline-input-description.h"

class RenderCommand; // Forward declaration to prevent circular inclusion

enum class AttributeBufferType {
	PER_VERTEX_ANY = 0,
	PER_INSTANCE_TRANSLATION,
	PER_INSTANCE_ENTITY_ID,
	PER_INSTANCE_MODEL_MAT
};

enum class AttributeBufferUsage {
	STATIC_DRAW = 0,
	DYNAMIC_DRAW
};

/**
 * @brief Vertex attribute buffer (points, uv mapping, normals, etc...)
 */
struct AttributeBuffer {
	unsigned int bufferId;
	unsigned int stride = 0;
	unsigned int count = 0;
	unsigned int byteWidth = 0;
	AttributeBufferUsage usage = AttributeBufferUsage::STATIC_DRAW;
	AttributeBufferType type = AttributeBufferType::PER_VERTEX_ANY;
};

/**
 * @brief Keep references to vertex attributes sent to GPU
 *
 * @note Attributes are stored in separate buffers (PPP)(TTT)(NNN).
 *		 This instead of interleaved (PTNPTNPTN) or packed (PPPTTTNNN).
	*/
struct VertexBuffer {
	std::vector<AttributeBuffer> buffers;
	unsigned int vertexArrayId;
};

/**
 * @brief Data used to prevent vertex duplication
 */
struct IndexBuffer {
	unsigned int bufferId;
	unsigned int count = 0;

	enum class dataType {
		UNSIGNED_BYTE, 
		UNSIGNED_SHORT,
		UNSIGNED_INT,
		NONE
	};
	dataType type = dataType::NONE;
};

/**
 * @brief Mesh used by cubes. The materials are indicated by each cube entity
 */
struct Mesh {
	VertexBuffer vb;
	IndexBuffer ib;
};

class Meshes {
public:
	Meshes() {};

	const Mesh& cube() const { return m_cube; }
	const Mesh& plane() const { return m_plane; }
	const Mesh& invertCube() const { return m_invertCube; }

private:
	void init(RenderCommand& rcommand);
	void destroy(RenderCommand& rcommand);

private:
	Mesh m_cube;
	Mesh m_plane;
	Mesh m_invertCube;

private:
	friend class App;
	friend class RenderSystem;
};
