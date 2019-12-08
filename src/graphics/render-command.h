#pragma once

#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif
#include <string>

#include "scomponents/singleton-components.h"
#include "components/graphics/pipeline.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/mesh.h"
#include "scomponents/graphics/render-targets.h"
#include "scomponents/graphics/texture.h"
#include "graphics/pipeline-input-description.h"
#include "graphics/pipeline-output-description.h"

class RenderCommand {
public:
    RenderCommand(SingletonComponents& scomps);
    ~RenderCommand();

    /**
	 * @brief Clear the back buffer
	 */
	void clear() const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// CREATION ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/**
	 * @param vertices - Array of data
	 * @param count - The number of elements in the array
	 * @param stride - Size in bytes of one element of the array
	 * @param usage - (Optional) Used to know if the buffer is going to be updated or not
	 * @param type - (Optional) Used by instanced rendering to know which buffer to update
	 */
	scomp::AttributeBuffer createAttributeBuffer(const void* vertices, unsigned int count, unsigned int stride, scomp::AttributeBufferUsage usage = scomp::AttributeBufferUsage::STATIC_DRAW, scomp::AttributeBufferType type = scomp::AttributeBufferType::PER_VERTEX_ANY) const;

	/**
	 * @param description - Layout of the buffers
	 * @param attributeBuffers - Array of buffers describing positions, normals, etc. The count is defined by the description size.
	 */
	scomp::VertexBuffer createVertexBuffer(const PipelineInputDescription& description, scomp::AttributeBuffer* attributeBuffers) const;

    /**
	 * @param indices - Array of integrer
	 * @param count - The number of elements in the array
	 * @param type - The type of integrer used in the array
	 */
	scomp::IndexBuffer createIndexBuffer(const void* indices, unsigned int count, scomp::IndexBuffer::dataType type) const;

	/**
	 * @param index - The type of constant buffer to create. It is used as an array index in singleton components
	 * @param byteWidth - The total size of the buffer. Allows to create an array of said type.
	 * @param data
	 */
	void createConstantBuffer(scomp::ConstantBufferIndex index, unsigned int byteWidth, void* data = nullptr) const;

	/**
	 * @brief Create a shader pipeline
	 */
	comp::Pipeline createPipeline(const char* VSfilePath, const char* FSfilePath, scomp::ConstantBufferIndex* cbIndices = nullptr, unsigned int cbCount = 0) const;

	/**
	 * @brief Allow a fragment shader to render to texture(s).
	 */
	scomp::RenderTargets createRenderTargets(const PipelineOutputDescription& description) const;

    ///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void bindVertexBuffer(const scomp::VertexBuffer& vb) const;
	void bindIndexBuffer(const scomp::IndexBuffer& ib) const;

	/**
	 * @brief Will bind all the shaders of the said pipeline
	 */
	void bindPipeline(const comp::Pipeline& pipeline) const;

	void bindRenderTargets(const scomp::RenderTargets rds) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UNBINDING ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void unbindRenderTargets() const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void updateConstantBuffer(const scomp::ConstantBuffer& cb, void* data) const;

	void updateAttributeBuffer(const scomp::AttributeBuffer& buffer, void* data, unsigned int dataByteWidth) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void drawIndexed(unsigned int count, scomp::IndexBuffer::dataType type) const;
	void drawIndexedInstances(unsigned int indexCount, scomp::IndexBuffer::dataType type, unsigned int drawCount) const;

private:
	bool hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const;
	GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) const;
	std::string readTextFile(const char* filePath) const;
	GLenum indexBufferDataTypeToOpenGLBaseType(scomp::IndexBuffer::dataType) const;

private:
	SingletonComponents& m_scomps;
};
