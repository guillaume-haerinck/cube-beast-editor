#include "render-command.h"

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <sstream>
#include <fstream>
#include <stb_image/stb_image.h>

#include "graphics/gl-exception.h"
#include "graphics/constant-buffer.h"

RenderCommand::RenderCommand(SingletonComponents& scomps) : m_scomps(scomps)
{
}

RenderCommand::~RenderCommand() {
    glDeleteVertexArrays(1, &m_scomps.cubeMesh.vb.vertexArrayId);
	for (auto buffer : m_scomps.cubeMesh.vb.buffers) {
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
}

void RenderCommand::clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////// CREATION ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

scomp::AttributeBuffer RenderCommand::createAttributeBuffer(const void* vertices, unsigned int count, unsigned int stride, scomp::AttributeBufferUsage usage, scomp::AttributeBufferType type) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
	GLenum glUsage;

	switch (usage) {
	case scomp::AttributeBufferUsage::STATIC_DRAW: glUsage = GL_STATIC_DRAW; break;
	case scomp::AttributeBufferUsage::DYNAMIC_DRAW:  glUsage = GL_DYNAMIC_DRAW; break;
	default:
		debug_break();
		assert(false && "[createAttributeBuffer] Unknow usage");
	}

	GLCall(glBufferData(GL_ARRAY_BUFFER, stride * count, vertices, glUsage));

	scomp::AttributeBuffer buffer = {};
	buffer.bufferId = id;
	buffer.byteWidth = stride * count;
	buffer.count = count;
	buffer.stride = stride;
	buffer.type = type;
	buffer.usage = usage;

	return buffer;
}

scomp::VertexBuffer RenderCommand::createVertexBuffer(const PipelineInputDescription& description, scomp::AttributeBuffer* attributeBuffers) const {
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

	scomp::VertexBuffer vb = {};
	vb.vertexArrayId = va;
	vb.buffers.assign(attributeBuffers, attributeBuffers + description.size());
	return vb;
}

scomp::IndexBuffer RenderCommand::createIndexBuffer(const void* indices, unsigned int count, scomp::IndexBuffer::dataType type) const {
	unsigned int id;
	GLCall(glGenBuffers(1, &id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	scomp::IndexBuffer buffer = {};
	buffer.bufferId = id;
	buffer.count = count;
	buffer.type = type;

	return buffer;
}

void RenderCommand::createConstantBuffer(scomp::ConstantBufferIndex index, unsigned int byteWidth, void* data) const {
    assert(byteWidth % 16 == 0 && "Constant Buffer byteWidth must be a multiple of 16 !");

	std::string name = "";
	switch (index) {
		case scomp::PER_FRAME: name = "perFrame"; break;
		//case scomp::PER_PHONG_MAT_CHANGE: name = "perPhongMatChange"; break;
		//case scomp::PER_COOK_MAT_CHANGE: name = "perCookMatChange"; break;
		//case scomp::PER_LIGHT_CHANGE: name = "perLightChange"; break;
		default:
			spdlog::error("[createConstantBuffer] unknown index {}", index);
            debug_break();
            assert(false);
		break;
	}
	
	unsigned int cbId = 0;
	GLCall(glGenBuffers(1, &cbId));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, cbId));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, byteWidth, data, GL_DYNAMIC_COPY));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	scomp::ConstantBuffer cb = {};
	cb.bufferId = cbId;
	cb.byteWidth = byteWidth;
	cb.name = name;

	// Save to singleton components
	m_scomps.constantBuffers.at(index) = cb;
}

void RenderCommand::createPipeline(scomp::PipelineIndex index, const char* VSfilePath, const char* FSfilePath, scomp::ConstantBufferIndex* cbIndices, unsigned int cbCount) const {
	// Compile vertex shader
	std::string shader = readTextFile(VSfilePath);
	const char* src = shader.c_str();
	unsigned int vsId = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vsId, 1, &src, nullptr));
	GLCall(glCompileShader(vsId));
	if (!hasShaderCompiled(vsId, GL_VERTEX_SHADER)) {
		spdlog::info("[Shader] {}", VSfilePath);
		spdlog::info("\n{}", shader);
		debug_break();
	}

	// Compile fragment shader
	shader = readTextFile(FSfilePath);
	src = shader.c_str();
	unsigned int fsId = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fsId, 1, &src, nullptr));
	GLCall(glCompileShader(fsId));
	if (!hasShaderCompiled(fsId, GL_FRAGMENT_SHADER)) {
		spdlog::info("[Shader] {}", FSfilePath);
		spdlog::info("\n{}", shader);
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
	GLint isLinked = 0;
	GLCall(glGetProgramiv(programId, GL_LINK_STATUS, (int*)&isLinked));
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		// FIXME
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(programId);

		spdlog::error("[createPipeline] Cannot link shader !");
		debug_break();
		assert(false);
	}

	// Link constant buffers
	scomp::Pipeline sPipeline = {};
	for (unsigned int i = 0; i < cbCount; i++) {
		scomp::ConstantBuffer& cb = m_scomps.constantBuffers.at(cbIndices[i]);
		unsigned int blockIndex = glGetUniformBlockIndex(programId, cb.name.c_str());
		GLCall(glUniformBlockBinding(programId, blockIndex, i));
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, i, cb.bufferId));
		sPipeline.constantBufferIndices.push_back(cbIndices[i]);
	}
	
	// Save to singleton components
	sPipeline.programIndex = programId;
	m_scomps.pipelines.at(index) = sPipeline;
}

void RenderCommand::createRenderTargets(scomp::RenderTargetsIndex index, const PipelineOutputDescription& description) const {
	// Create new framebuffer
	unsigned int fb;
	GLCall(glGenFramebuffers(1, &fb));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fb));

    unsigned int slot = 0;
    for (const auto& target : description) {
        unsigned int rbo;
        unsigned int textureId;

        switch (target.type) {
        case RenderTargetType::Texture:
            GLCall(glGenTextures(1, &textureId));
            GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            break;

        case RenderTargetType::RenderBuffer:
            GLCall(glGenRenderbuffers(1, &rbo));
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
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
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, 0)); // TODO get width and height of window from scomps
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, textureId, 0));
            } else if (target.type == RenderTargetType::RenderBuffer) {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 500, 500)); // TODO get width and height of window from scomps
                GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_RENDERBUFFER, rbo));
            }
            slot++;
            break;
   
        case RenderTargetUsage::Depth:
            if (target.type == RenderTargetType::Texture) {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 500, 500, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0)); // TODO get width and height of window from scomps
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0));
            } else if (target.type == RenderTargetType::RenderBuffer) {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 500, 500)); // TODO get width and height of window from scomps
                GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo));
            }
            break;

        case RenderTargetUsage::DepthStencil:
            if (target.type == RenderTargetType::Texture) {
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 500, 500, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0)); // TODO get width and height of window from scomps
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0));
            } else if (target.type == RenderTargetType::RenderBuffer) {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 500, 500)); // TODO get width and height of window from scomps
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
	
	// Check for errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		spdlog::error("[createRenderTarget] FrameBuffer is not complete !");
		debug_break();
		assert(false);
	}

	// Unbind
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // Assign to singleton components
	scomp::RenderTargets rts;
	rts.frameBufferId = fb;
    m_scomps.renderTargets.at(index) = rts;
}

///////////////////////////////////////////////////////////////////////////
////////////////////////////////// BINDING ////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void RenderCommand::bindVertexBuffer(const scomp::VertexBuffer& vb) const {
	GLCall(glBindVertexArray(vb.vertexArrayId));
}

void RenderCommand::bindIndexBuffer(const scomp::IndexBuffer& ib) const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.bufferId));
}

void RenderCommand::bindPipeline(const scomp::Pipeline& pipeline) const {
	GLCall(glUseProgram(pipeline.programIndex));
}

void RenderCommand::bindRenderTargets(const scomp::RenderTargets rds) const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, rds.frameBufferId));
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

void RenderCommand::updateConstantBuffer(const scomp::ConstantBuffer& cb, void* data) const {
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, cb.bufferId));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, cb.byteWidth, data));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void RenderCommand::updateAttributeBuffer(const scomp::AttributeBuffer& buffer, void* data, unsigned int dataByteWidth) const {
	assert(dataByteWidth <= buffer.byteWidth && "New attribute buffer data exceed the size of the allocated buffer");
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer.bufferId));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dataByteWidth, data));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////// DRAWING /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void RenderCommand::drawIndexed(unsigned int count, scomp::IndexBuffer::dataType type) const {
	GLCall(glDrawElements(GL_TRIANGLES, count, indexBufferDataTypeToOpenGLBaseType(type), (void*) 0));
}

void RenderCommand::drawIndexedInstances(unsigned int indexCount, scomp::IndexBuffer::dataType type, unsigned int drawCount) const {
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
	case ShaderDataType::Bool:     return GL_BOOL;
	case ShaderDataType::None:	   break;
	}

	assert(false && "Unknown ShaderDataType!");
	return 0;
}

GLenum RenderCommand::indexBufferDataTypeToOpenGLBaseType(scomp::IndexBuffer::dataType type) const {
	switch (type) {
	case scomp::IndexBuffer::dataType::UNSIGNED_BYTE : return GL_UNSIGNED_BYTE;
	case scomp::IndexBuffer::dataType::UNSIGNED_SHORT : return GL_UNSIGNED_SHORT; 
	case scomp::IndexBuffer::dataType::UNSIGNED_INT : return GL_UNSIGNED_INT;
	default:	break;
	}

	assert(false && "Unknown indexBufferDataType!");
	return 0;
}

std::string RenderCommand::readTextFile(const char* filePath) const {
	std::ifstream fileStream(filePath);
	if (!fileStream) {
		spdlog::error("[readTextFile] Cannot load file : {}", filePath);
		debug_break();
		assert(false);
	}

	std::stringstream shaderStream;
	shaderStream << fileStream.rdbuf();
	fileStream.close();
	return shaderStream.str();
}
