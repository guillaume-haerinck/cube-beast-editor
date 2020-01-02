#pragma once

#include <assert.h>
#include <debug_break/debug_break.h>
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/gles2.h>
#endif

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

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
    #define OGL_SCOPE(x)
#else
    #define OGL_SCOPE(name) glexp::DebugGroup CONCAT(dgroup, __LINE__)(name)
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

#ifndef __EMSCRIPTEN__
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
    void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* userParam);

    /**
     * @brief Groups next glCalls inside a name to help debugging.
     */
    class DebugGroup {
    public:
        DebugGroup(const char* name) {
            glPushDebugGroupKHR(GL_DEBUG_SOURCE_APPLICATION_KHR, 0, -1, name);
        }

        ~DebugGroup() {
            glPopDebugGroupKHR();
        }
    };
#endif

}
