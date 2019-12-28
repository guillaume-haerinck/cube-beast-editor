#include "render-command.h"

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

#include "graphics/gl-exception.h"
#include "graphics/constant-buffer.h"

RenderCommand::RenderCommand() {}

RenderCommand::~RenderCommand() {
	/*
    glDeleteVertexArrays(1, &m_scomps.cubeMesh.vb.vertexArrayId);
	for (auto buffer : m_scomps.cubeMesh.vb.buffers) {
        glDeleteBuffers(1, &buffer.bufferId);
	}

	glDeleteVertexArrays(1, &m_scomps.planeMesh.vb.vertexArrayId);
	for (auto buffer : m_scomps.planeMesh.vb.buffers) {
        glDeleteBuffers(1, &buffer.bufferId);
	}

	glDeleteVertexArrays(1, &m_scomps.invertCubeMesh.vb.vertexArrayId);
	for (auto buffer : m_scomps.invertCubeMesh.vb.buffers) {
        glDeleteBuffers(1, &buffer.bufferId);
	}

	for (auto pipeline : m_scomps.pipelines) {
		glDeleteProgram(pipeline.programIndex);
	}

	for (auto cb : m_scomps.constantBuffers) {
		glDeleteBuffers(1, &cb.bufferId);
	}

    for (auto rts : m_scomps.renderTargets) {
        glDeleteFramebuffers(1, &rts.frameBufferId);
    }
	*/
}

void RenderCommand::clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderCommand::enableDepthTest() const {
	GLCall(glEnable(GL_DEPTH_TEST));
}

void RenderCommand::disableDepthTest() const {
	GLCall(glDisable(GL_DEPTH_TEST));
}

void RenderCommand::enableFaceCulling() const {
	GLCall(glEnable(GL_CULL_FACE));
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////// CREATION ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

AttributeBuffer RenderCommand::createAttributeBuffer(const void* vertices, unsigned int count, unsigned int stride, AttributeBufferUsage usage, AttributeBufferType type) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ARRAY_BUFFER, stride * count, vertices, attributeBufferUsageToOpenGLBaseType(usage)));

	AttributeBuffer buffer = {};
	buffer.bufferId = id;
	buffer.byteWidth = stride * count;
	buffer.count = count;
	buffer.stride = stride;
	buffer.type = type;
	buffer.usage = usage;

	return buffer;
}

VertexBuffer RenderCommand::createVertexBuffer(const PipelineInputDescription& description, AttributeBuffer* attributeBuffers) const {
	GLuint va;
	GLCall(glGenVertexArrays(1, &va));
	GLCall(glBindVertexArray(va));

	// Set layout
	unsigned int vbIndex = 0;
	unsigned int elementId = 0;
	for (const auto& element : description) {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, attributeBuffers[elementId].bufferId));

		unsigned int iter = 1;
		if (element.type == ShaderDataType::Mat3)
			iter = 3;
		else if (element.type == ShaderDataType::Mat4)
			iter = 4;

		for (unsigned int i = 0; i < iter; i++) {
			GLCall(glEnableVertexAttribArray(vbIndex + i));
			GLCall(glVertexAttribPointer(
				vbIndex + i,
				element.getComponentCount() / iter,
				shaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				element.size,
				(const void*) ((element.size / iter) * i)
			));
			if (element.usage == BufferElementUsage::PerInstance) {
				GLCall(glVertexAttribDivisor(vbIndex + i, 1));
			}
		}
		elementId++;
		vbIndex += iter;
	}

	GLCall(glBindVertexArray(0));

	VertexBuffer vb = {};
	vb.vertexArrayId = va;
	vb.buffers.assign(attributeBuffers, attributeBuffers + description.size());
	return vb;
}

IndexBuffer RenderCommand::createIndexBuffer(const void* indices, unsigned int count, IndexBuffer::dataType type) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	IndexBuffer buffer = {};
	buffer.bufferId = id;
	buffer.count = count;
	buffer.type = type;

	return buffer;
}

ConstantBuffer RenderCommand::createConstantBuffer(const char* name, unsigned int byteWidth, void* data) const {
    assert(byteWidth % 16 == 0 && "Constant Buffer byteWidth must be a multiple of 16 !");
	
	unsigned int cbId = 0;
	GLCall(glGenBuffers(1, &cbId));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, cbId));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, byteWidth, data, GL_DYNAMIC_COPY));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	ConstantBuffer cb = {};
	cb.bufferId = cbId;
	cb.byteWidth = byteWidth;
	cb.name = name;
	return cb;
}

Pipeline RenderCommand::createPipeline(const char* vsSrc, const char* fsSrc, const std::vector<ConstantBuffer>& cbs, const std::vector<std::string>& samplerNames) const {
	// Compile vertex shader
	unsigned int vsId = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vsId, 1, &vsSrc, nullptr));
	GLCall(glCompileShader(vsId));
	if (!hasShaderCompiled(vsId, GL_VERTEX_SHADER)) {
		spdlog::info("{}", vsSrc);
		debug_break();
	}

	// Compile fragment shader
	unsigned int fsId = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fsId, 1, &fsSrc, nullptr));
	GLCall(glCompileShader(fsId));
	if (!hasShaderCompiled(fsId, GL_FRAGMENT_SHADER)) {
		spdlog::info("\n{}", fsSrc);
		debug_break();
	}

	// Compile pipeline
	unsigned int programId = glCreateProgram();
	GLCall(glAttachShader(programId, vsId));
	GLCall(glAttachShader(programId, fsId));
	GLCall(glLinkProgram(programId));
	GLCall(glDeleteShader(vsId));
	GLCall(glDeleteShader(fsId));
	
	// Check fs and vs link errors
	GLCall(glValidateProgram(programId));
	unsigned int isLinked = 0;
	GLCall(glGetProgramiv(programId, GL_LINK_STATUS, (int*)&isLinked));
	if (isLinked == GL_FALSE) {
        spdlog::error("[createPipeline] Cannot link shaders");

        int length;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            std::vector<char> message(length + 1);
            glGetProgramInfoLog(programId, length, &length, &message[0]);
            std::string e(message.begin(), message.end());
            spdlog::error("{}", e);
        } else {
			spdlog::info("'{} /n /n {}'", vsSrc, fsSrc);
		}

        glDeleteProgram(programId);
		debug_break();
		assert(false);
	}

	// Link constant buffers
	// FIXME problems when assigning multiple pipelines next to each other
	// the associated cb are not correct
	GLCall(glUseProgram(programId));
	Pipeline sPipeline = {};
	unsigned int index = 0;
	for (const ConstantBuffer& cb : cbs) {
		unsigned int blockIndex = glGetUniformBlockIndex(programId, cb.name.c_str());
		GLCall(glUniformBlockBinding(programId, blockIndex, index));
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, index, cb.bufferId));
		sPipeline.cbNames.push_back(cb.name);
		index++;
	}

	// Set samplers texture units to the order they were declared
	for (size_t i = 0; i < samplerNames.size(); i++) {
		int samplerLocation = glGetUniformLocation(programId, samplerNames.at(i).c_str());
		if (samplerLocation != -1) {
			GLCall(glUniform1i(samplerLocation, i));
		} else {
			spdlog::error("[createPipeline] Sampler '{}' doesn't exist, or is not used. It must affect the fragment shader output variable !", samplerNames.at(i));
			debug_break();
			assert(false && "Sampler name doesn't exist !");
		}
	}

	// Unbind
	GLCall(glUseProgram(0));
	
	// Save to singleton components
	sPipeline.programIndex = programId;
	return sPipeline;
}

RenderTarget RenderCommand::createRenderTarget(const PipelineOutputDescription& description, const glm::ivec2& size) const {
	// Create new framebuffer
	unsigned int fb;
	GLCall(glGenFramebuffers(1, &fb));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fb));

    unsigned int slot = 0;
	RenderTarget rt;
    for (const auto& target : description) {
        unsigned int rbo;
        unsigned int textureId;

        switch (target.type) {
        case RenderTargetType::Texture:
            GLCall(glGenTextures(1, &textureId));
            GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			rt.textureIds.push_back(textureId);
            break;

        case RenderTargetType::RenderBuffer:
            GLCall(glGenRenderbuffers(1, &rbo));
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
			rt.renderBufferIds.push_back(rbo);
            break;

        default:
            spdlog::error("[createRenderTarget] Unknown render target type");
            debug_break();
            assert(false);
            break;
        }

        switch (target.usage) {
        case RenderTargetUsage::Color:
            if (target.type == RenderTargetType::Texture) {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, renderTargetChannelsToOpenGLInternalFormat(target.channels, target.dataType), size.x, size.y, 0, renderTargetChannelsToOpenGLBaseFormat(target.channels), renderTargetDataTypeToOpenGLBaseType(target.dataType), 0));
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, textureId, 0));
            } else if (target.type == RenderTargetType::RenderBuffer) {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, renderTargetChannelsToOpenGLInternalFormat(target.channels, target.dataType), size.x, size.y));
                GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_RENDERBUFFER, rbo));
            }
            slot++;
            break;
   
        case RenderTargetUsage::Depth:
            if (target.type == RenderTargetType::Texture) {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, size.x, size.y, 0, GL_DEPTH_COMPONENT, renderTargetDataTypeToOpenGLBaseType(target.dataType), 0));
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0));
            } else if (target.type == RenderTargetType::RenderBuffer) {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.x, size.y));
                GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo));
            }
            break;

        case RenderTargetUsage::DepthStencil:
            if (target.type == RenderTargetType::Texture) {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0));
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0));
            } else if (target.type == RenderTargetType::RenderBuffer) {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y));
                GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
            }
            break;

        default:
            spdlog::error("[createRenderTarget] Unknown render target usage");
            debug_break();
            assert(false);
            break;
        }
    }

	// Attach color targets to framebuffer
	std::vector<unsigned int> attachments(slot);
	for (unsigned int i = 0; i < slot; i++) {
		attachments.at(i) = GL_COLOR_ATTACHMENT0 +  i;
	}
	GLCall(glDrawBuffers(slot, attachments.data()));
	
	// Check for errors
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status!= GL_FRAMEBUFFER_COMPLETE) {
		std::string detail = "";
		switch (status) {
		case GL_FRAMEBUFFER_UNDEFINED: detail = "undefined"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: detail = "Incomplete attachment"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: detail = "Incomplete missing attachment"; break;
		case GL_FRAMEBUFFER_UNSUPPORTED: detail = "unsupported"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: detail = "Incomplete multisample"; break;
		default:
			break;
		}

		spdlog::error("[createRenderTarget] FrameBuffer is not complete : {}", detail);
		debug_break();
		assert(false);
	}

	// Unbind
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // Assign to singleton components
	rt.frameBufferId = fb;
    return rt;
}

///////////////////////////////////////////////////////////////////////////
////////////////////////////////// BINDING ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void RenderCommand::bindVertexBuffer(const VertexBuffer& vb) const {
	GLCall(glBindVertexArray(vb.vertexArrayId));
}

void RenderCommand::bindIndexBuffer(const IndexBuffer& ib) const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.bufferId));
}

void RenderCommand::bindTextures(const std::vector<unsigned int>& textureIds) const {
	for (size_t i = 0; i < textureIds.size(); i++) {
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
        GLCall(glBindTexture(GL_TEXTURE_2D, textureIds.at(i)));
	}
}

void RenderCommand::bindPipeline(const Pipeline& pipeline) const {
	GLCall(glUseProgram(pipeline.programIndex));
}

void RenderCommand::bindRenderTargets(const RenderTargets rds) const {
    //GLCall(glBindFramebuffer(GL_FRAMEBUFFER, rds.frameBufferId));
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////// UNBINDING ///////////////////////////////
///////////////////////////////////////////////////////////////////////////

void RenderCommand::unbindRenderTargets() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void RenderCommand::unbindVertexBuffer() const {
    GLCall(glBindVertexArray(0));
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////// UPDATING ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void RenderCommand::updateConstantBuffer(const ConstantBuffer& cb, void* data) const {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, cb.bufferId));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, cb.byteWidth, data));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void RenderCommand::updateAttributeBuffer(const AttributeBuffer& buffer, void* data, unsigned int dataByteWidth) const {
	assert(dataByteWidth < buffer.byteWidth && "New attribute buffer data exceed the size of the allocated buffer");
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer.bufferId));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dataByteWidth, data));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void RenderCommand::updateAttributeBufferAnySize(AttributeBuffer& buffer, void* data, unsigned int dataByteWidth) const {
	if (dataByteWidth >= buffer.byteWidth) {
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer.bufferId));
		GLCall(glBufferData(GL_ARRAY_BUFFER, dataByteWidth * 2, 0, attributeBufferUsageToOpenGLBaseType(buffer.usage)));
		buffer.byteWidth = dataByteWidth * 2;
	}
	
	updateAttributeBuffer(buffer, data, dataByteWidth);
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////// DRAWING /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void RenderCommand::drawLines(unsigned int count) const {
	GLCall(glDrawArrays(GL_LINES, 0, count));
}

void RenderCommand::drawIndexed(unsigned int count, IndexBuffer::dataType type) const {
	GLCall(glDrawElements(GL_TRIANGLES, count, indexBufferDataTypeToOpenGLBaseType(type), (void*) 0));
}

void RenderCommand::drawIndexedInstances(unsigned int indexCount, IndexBuffer::dataType type, unsigned int drawCount) const {
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, indexCount, indexBufferDataTypeToOpenGLBaseType(type), (void*)0, drawCount));
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////// PRIVATE METHODS /////////////////////////////
///////////////////////////////////////////////////////////////////////////

bool RenderCommand::hasShaderCompiled(unsigned int shaderId, unsigned int shaderType) const {
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		auto const typeString = [shaderType]() {
			switch (shaderType) {
			case GL_VERTEX_SHADER: return "vertex";
			case GL_FRAGMENT_SHADER: return "fragment";
			default: return "unknown type";
			}
		}();

		spdlog::error("[Shader] Failed to compile {} shader", typeString);
		spdlog::error("[Shader] {}", message);
		GLCall(glDeleteShader(shaderId));
		return false;
	}

	return true;
}

GLenum RenderCommand::shaderDataTypeToOpenGLBaseType(ShaderDataType type) const {
	switch (type) {
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::UInt:     return GL_UNSIGNED_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	assert(false && "Unknown ShaderDataType!");
	return 0;
}

GLenum RenderCommand::indexBufferDataTypeToOpenGLBaseType(IndexBuffer::dataType type) const {
	switch (type) {
	case IndexBuffer::dataType::UNSIGNED_BYTE : return GL_UNSIGNED_BYTE;
	case IndexBuffer::dataType::UNSIGNED_SHORT : return GL_UNSIGNED_SHORT; 
	case IndexBuffer::dataType::UNSIGNED_INT : return GL_UNSIGNED_INT;
	default:	break;
	}

	assert(false && "Unknown indexBufferDataType!");
	return 0;
}

GLenum RenderCommand::renderTargetChannelsToOpenGLInternalFormat(RenderTargetChannels channels, RenderTargetDataType dataType) const {
	if (dataType == RenderTargetDataType::UCHAR) {
		switch (channels) {
		case RenderTargetChannels::R : 		return GL_R8;
		case RenderTargetChannels::RG : 	return GL_RG8;
		case RenderTargetChannels::RGB : 	return GL_RGB8;
		case RenderTargetChannels::RGBA : 	return GL_RGBA8;
		default:	break;
		}
	} else if (dataType == RenderTargetDataType::FLOAT) {
		switch (channels) {
		case RenderTargetChannels::R : 		return GL_R16F;
		case RenderTargetChannels::RG : 	return GL_RG16F;
		case RenderTargetChannels::RGB : 	return GL_RGB16F;
		case RenderTargetChannels::RGBA : 	return GL_RGBA16F;
		default:	break;
		}
	}

	assert(false && "Unknown RenderTargetChannels type !");
	return 0;
}

GLenum RenderCommand::renderTargetChannelsToOpenGLBaseFormat(RenderTargetChannels channels) const {
	switch (channels) {
	case RenderTargetChannels::R : 		return GL_RED;
	case RenderTargetChannels::RG : 	return GL_RG;
	case RenderTargetChannels::RGB : 	return GL_RGB;
	case RenderTargetChannels::RGBA : 	return GL_RGBA;
	default:	break;
	}

	assert(false && "Unknown RenderTargetChannels type !");
	return 0;
}

GLenum RenderCommand::renderTargetDataTypeToOpenGLBaseType(RenderTargetDataType dataType) const {
	switch (dataType) {
		case RenderTargetDataType::UCHAR : return GL_UNSIGNED_BYTE;
		case RenderTargetDataType::FLOAT : return GL_FLOAT;
		default: break;
	}

	assert(false && "Unknown RenderTargetDataType !");
	return 0;
}

GLenum RenderCommand::attributeBufferUsageToOpenGLBaseType(AttributeBufferUsage usage) const {
	switch (usage) {
	case AttributeBufferUsage::STATIC_DRAW: return GL_STATIC_DRAW;
	case AttributeBufferUsage::DYNAMIC_DRAW:  return GL_DYNAMIC_DRAW;
	default:
		break;
	}

	assert(false && "[createAttributeBuffer] Unknow usage");
	return 0;
}


