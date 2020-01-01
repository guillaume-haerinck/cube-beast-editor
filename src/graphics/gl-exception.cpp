#include "gl-exception.h"

#include <spdlog/spdlog.h>

void glexp::clear() {
	while (glGetError() != GL_NO_ERROR);
}

bool glexp::doesFunctionWorks(const char *functionName, const char *filename, int line) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		spdlog::error("[OpenGL Error] {}: {} {} {}", glErrorString(error), functionName, filename, line);
		return false;
	}
	return true;
}

char const* glexp::glErrorString(GLenum const err) {
	switch (err) {
	case GL_NO_ERROR:
		return "GL_NO_ERROR";

	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";

	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";

	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";

	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";

	default:
		assert(!"unknown error");
		return nullptr;
	}
}

#ifndef __EMSCRIPTEN__
void GLAPIENTRY glexp::messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param) {
	auto const sourceString = [source]() {
		switch (source) {
			case GL_DEBUG_SOURCE_API_KHR: return "Api";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR: return "Window system";
			case GL_DEBUG_SOURCE_SHADER_COMPILER_KHR: return "Shader compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY_KHR: return "Third party";
			case GL_DEBUG_SOURCE_APPLICATION_KHR: return "Application";
			case GL_DEBUG_SOURCE_OTHER_KHR: return "Other";
		}
	}();

	auto const typeString = [type]() {
		switch (type) {
			case GL_DEBUG_TYPE_ERROR_KHR: return "error";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR: return "deprecated behavior";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR: return "undefined behavior";
			case GL_DEBUG_TYPE_PORTABILITY_KHR: return "portability";
			case GL_DEBUG_TYPE_PERFORMANCE_KHR: return "performance";
			case GL_DEBUG_TYPE_MARKER_KHR: return "marker";
			case GL_DEBUG_TYPE_OTHER_KHR: return "other";
		}
	}();

	switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
			spdlog::info("[{} {}] {} : {}", sourceString, typeString, id, message);
			break;

		case GL_DEBUG_SEVERITY_LOW_KHR:
			spdlog::warn("[{} {}] {} : {}", sourceString, typeString, id, message);
			break;

		case GL_DEBUG_SEVERITY_MEDIUM_KHR:
			spdlog::error("[{} {}] {} : {}", sourceString, typeString, id, message);
			break;

		case GL_DEBUG_SEVERITY_HIGH_KHR:
			spdlog::critical("[{} {}] {} : {}", sourceString, typeString, id, message);
			break;
	}
}
#endif // __EMSCRIPTEN__
