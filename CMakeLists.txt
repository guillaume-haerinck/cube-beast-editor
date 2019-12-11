# Set standards
cmake_minimum_required(VERSION 2.8)
project(voxel-editor C CXX)
set(CMAKE_CXX_STANDARD 17)

# Setup Wasm build
if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1")
endif()

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////////// PROJECT FILES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

file(GLOB_RECURSE MY_SOURCES src/*)

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////// DEPENDENCIES ////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

find_package(OpenGL REQUIRED)

# On windows
if (WIN32) 
    set(SDL2_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.10/include)
    set(SDL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2.lib;
                       ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2main.lib)

# On linux
elseif (NOT EMSCRIPTEN)
    find_package(SDL2 REQUIRED)
endif()

# ------------------------------ BUILD IF NEEDED ------------------------------

if (NOT EMSCRIPTEN)
    add_library(GLAD STATIC "lib/glad/src/gles2.c")
endif()
file(GLOB_RECURSE IMGUI_SOURCES lib/imgui/*)
add_library(IMGUI STATIC ${IMGUI_SOURCES})

# /////////////////////////////////////////////////////////////////////////////
# ///////////////////////////////// INCLUDES //////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

include_directories(
    ${OPENGL_INCLUDE_DIR}
    lib/imgui
    lib
    src
    res
)

if (NOT EMSCRIPTEN)
    include_directories(
        ${SDL2_INCLUDE_DIRS}
        lib/glad/include
    )
endif()

# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////////// BUILD ///////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(${PROJECT_NAME} ${MY_SOURCES})
target_link_libraries(
    ${PROJECT_NAME}
    IMGUI
)

if (NOT EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} GLAD)
endif()

if (UNIX)
    target_link_libraries(${PROJECT_NAME} -ldl)
endif()

if (WIN32) # Copy .dll to build folder
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/SDL2-2.0.10/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()


# /////////////////////////////////////////////////////////////////////////////
# /////////////////////////////////// TESTS ///////////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

if (NOT EMSCRIPTEN)
    file(GLOB_RECURSE MY_TESTS test/*)
	file(GLOB_RECURSE MY_MATHS src/maths/*)
    add_executable(${PROJECT_NAME}-tests ${MY_TESTS} ${MY_MATHS})
endif()
