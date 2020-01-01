#pragma once

#include <assert.h>
#include <debug_break/debug_break.h>
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif

/**
 * @brief Assertion and logger handling for opengl functions
 */
#if defined(NDEBUG)
    #define GLCall(x) x
#else
    #define break_assert(x) if (!x) { debug_break(); assert(false); }
    #define GLCall(x) glexp::clear(); x; break_assert(glexp::doesFunctionWorks(#x, __FILE__, __LINE__))
#endif

/**
 * @brief Send event to group openGl calls
 */
#if defined(NDEBUG) || defined(__EMSCRIPTEN__)
    #define startDebugEvent(x)
    #define endDebugEvent()
#else
    #define startDebugEvent(x) glPushDebugGroupKHR(GL_DEBUG_SOURCE_APPLICATION_KHR, 0, -1, x)
    #define endDebugEvent() glPopDebugGroupKHR()
#endif

namespace glexp {
    /**
     * @brief Empty the OpenGl error buffer
     */
    void clear();

    /**
     * @brief Print OpenGl errors to the console if any
     * 
     * @param functionName
     * @param filename
     * @param line
     */
    bool doesFunctionWorks(const char* functionName, const char* filename, int line);

    /**
     * @brief Transform error enum to text for OpenGL
     * 
     * @param err 
     * @return char const* - Text corresponding to OpenGl error
     */
    char const* glErrorString(GLenum const err);

    /**
     * @brief Detailed log of open gl
     * @note You must have the GL_KHR_debug extension, and use glEnable(GL_DEBUG_OUTPUT_KHR) and glDebugMessageCallback(debugCallback, nullptr) to get it working.
     * 
     * @param source 
     * @param type 
     * @param id 
     * @param severity 
     * @param length 
     * @param message 
     * @param user_param 
     */
#ifndef __EMSCRIPTEN__
    void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* userParam);
#endif

}
