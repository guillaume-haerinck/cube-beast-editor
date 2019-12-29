#pragma once

#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif
#include <string>

#include "scomponents/singleton-components.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/meshes.h"
#include "scomponents/graphics/render-targets.h"
#include "scomponents/graphics/textures.h"
#include "graphics/pipeline-input-description.h"
#include "graphics/pipeline-output-description.h"

class RenderCommand {
public:
    RenderCommand();
    ~RenderCommand();

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// FEATURES ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    /**
	 * @brief Clear the back buffer
	 */
	void clear() const;

	void enableDepthTest() const;
	void disableDepthTest() const;

	void enableFaceCulling() const;

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
	AttributeBuffer createAttributeBuffer(const void* vertices, unsigned int count, unsigned int stride, AttributeBufferUsage usage = AttributeBufferUsage::STATIC_DRAW, AttributeBufferType type = AttributeBufferType::PER_VERTEX_ANY) const;

	/**
	 * @param description - Layout of the buffers
	 * @param attributeBuffers - Array of buffers describing positions, normals, etc. The count is defined by the description size.
	 */
	VertexBuffer createVertexBuffer(const PipelineInputDescription& description, AttributeBuffer* attributeBuffers) const;

    /**
	 * @param indices - Array of integrer
	 * @param count - The number of elements in the array
	 * @param type - The type of integrer used in the array
	 */
	IndexBuffer createIndexBuffer(const void* indices, unsigned int count, IndexBuffer::dataType type) const;

	/**
	 * @param byteWidth - The total size of the buffer. Allows to create an array of said type.
	 * @param data
	 */
	ConstantBuffer createConstantBuffer(const char* name, unsigned int bindingPoint, unsigned int byteWidth, void* data = nullptr) const;

	/**
	 * @brief Create a shader pipeline
	 */
	Pipeline createPipeline(const char* VSfilePath, const char* FSfilePath, const std::vector<ConstantBuffer>& cbs, const std::vector<std::string>& samplerNames = {}) const;

	/**
	 * @brief Allow a fragment shader to render to texture(s).
	 */
	RenderTarget createRenderTarget(const PipelineOutputDescription& description, const glm::ivec2& size) const;

	Texture createTexture(unsigned int slot, const unsigned char* data, unsigned int dataSize) const;

    ///////////////////////////////////////////////////////////////////////////
	////////////////////////////////// BINDING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void bindVertexBuffer(const VertexBuffer& vb) const;
	void bindIndexBuffer(const IndexBuffer& ib) const;
	void bindTexture(const Texture& tex) const;
	void bindTextureIds(const std::vector<unsigned int>& textureIds) const;

	/**
	 * @brief Will bind all the shaders of the said pipeline
	 */
	void bindPipeline(const Pipeline& pipeline) const;

	void bindRenderTarget(const RenderTarget rt) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UNBINDING ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void unbindRenderTargets() const;

    void unbindVertexBuffer() const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// UPDATING ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

    void updateConstantBuffer(const ConstantBuffer& cb, const void* data, unsigned int dataByteWidth) const;

	void updateAttributeBuffer(const AttributeBuffer& buffer, const void* data, unsigned int dataByteWidth) const;

	void updateAttributeBufferAnySize(AttributeBuffer& buffer, const void* data, unsigned int dataByteWidth) const;

    ///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DRAWING /////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void drawLines(unsigned int count) const;

	void drawIndexed(unsigned int count, IndexBuffer::dataType type) const;
	void drawIndexedInstances(unsigned int indexCount, IndexBuffer::dataType type, unsigned int drawCount) const;

	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////// DELETION ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	void deleteRenderTarget(RenderTarget& rt) const;
	void deleteVertexBuffer(VertexBuffer& vb) const;
	void deleteConstantBuffer(ConstantBuffer& cb) const;
	void deleteIndexBuffer(IndexBuffer& ib) const;
	void deleteTexture(Texture& texture) const;
	void deletePipeline(Pipeline& pip) const;

private:
	bool hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const;
	bool isShaderDataTypeIntegrer(ShaderDataType type) const;
	GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) const;
	GLenum indexBufferDataTypeToOpenGLBaseType(IndexBuffer::dataType) const;
	GLenum renderTargetChannelsToOpenGLInternalFormat(RenderTargetChannels channels, RenderTargetDataType dataType) const;
	GLenum renderTargetChannelsToOpenGLBaseFormat(RenderTargetChannels channels) const;
	GLenum renderTargetDataTypeToOpenGLBaseType(RenderTargetDataType dataType) const;
	GLenum attributeBufferUsageToOpenGLBaseType(AttributeBufferUsage usage) const;
};
