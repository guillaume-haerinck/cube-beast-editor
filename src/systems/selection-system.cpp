#include "selection-system.h"

// Temp
#ifdef __EMSCRIPTEN__
    #include <GLES3/gl3.h>
#else
    #include <glad/gles2.h>
#endif

#include <spdlog/spdlog.h>
#include "graphics/gl-exception.h"
#include "maths/casting.h"

SelectionSystem::SelectionSystem(Context& ctx, SingletonComponents& scomps) : m_ctx(ctx), m_scomps(scomps) {}

SelectionSystem::~SelectionSystem() {}

void SelectionSystem::update() {
    // TODO cleanup
    m_ctx.rcommand.bindRenderTargets(m_scomps.renderTargets.at(scomp::RenderTargetsIndex::RTT_GEOMETRY));

    // TODO abstract
    // FIXME glReadPixels does not work on some computers
    unsigned char pixel[] = { 0, 0, 0, 0 };
    GLCall(glReadBuffer(GL_COLOR_ATTACHMENT3));
    GLCall(glReadPixels(m_scomps.inputs.mousePos.x, 500 - m_scomps.inputs.mousePos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixel));

    m_scomps.hoveredCube.id = voxmt::colorToInt(pixel[0], pixel[1], pixel[2]);
    m_scomps.hoveredCube.face = colorToFace(pixel[3]);

    
    ///////////////// TODO raycast ////////////////
    // http://antongerdelan.net/opengl/raycasting.html

    // Camera ray to different coordinate systems
    glm::vec4 ray_clip = glm::vec4(m_scomps.inputs.NDCMousePos, -1.0, 1.0);
    glm::vec4 ray_eye = glm::inverse(m_scomps.camera.proj) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
    glm::vec4 ray_worldW = (glm::inverse(m_scomps.camera.view) * ray_eye);
    ray_worldW = glm::normalize(ray_worldW);
    glm::vec3 ray_world = glm::vec3(ray_worldW.x, ray_worldW.y, ray_worldW.z);

    //spdlog::info("Ray in world coord : {} {} {}", ray_world.x, ray_world.y, ray_world.z);

    // TODO ray_world against plane of size 1 at 1, 1, 1 intersection.
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    glm::vec3 n = glm::vec3(-1, 0, 0); // Left plane normal from camera start
    glm::vec3 p0 = glm::vec3(0, 0, 0);

    std::vector<glm::vec3> lines = {
        glm::vec3(0, 0, 0)
    };

    m_ctx.ddraw.addLines(lines);

    float perp = glm::dot(ray_world, n);
    if (perp < 0) {
        spdlog::info("Facing");
    } else {
        spdlog::info("behind");
    }

}

scomp::Face SelectionSystem::colorToFace(unsigned char color) const {
    switch (color) {
        case 0: return scomp::Face::NONE;
        case 1: return scomp::Face::BACK;
        case 2: return scomp::Face::RIGHT;
        case 3: return scomp::Face::TOP;
        case 4: return scomp::Face::FRONT;
        case 5: return scomp::Face::LEFT;
        case 6: return scomp::Face::BOTTOM;
        default:
            debug_break();
            assert(false && "Unknown face picked");
            return scomp::Face::NONE;
    }
}
