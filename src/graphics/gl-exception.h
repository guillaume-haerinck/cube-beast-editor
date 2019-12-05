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
#ifndef NDEBUG
    #define break_assert(x) if (!x) { debug_break(); assert(false); }
    #define GLCall(x) glexp::clear(); x; break_assert(glexp::doesFunctionWorks(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
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
}
