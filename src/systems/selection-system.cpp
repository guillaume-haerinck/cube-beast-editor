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
    
    ///////////////// raycast ////////////////
    // http://antongerdelan.net/opengl/raycasting.html
    // https://stackoverflow.com/questions/20140711/picking-in-3d-with-ray-tracing-using-ninevehgl-or-opengl-i-phone
    // https://www.youtube.com/watch?v=fQOeEA_8-u

    glm::mat4 toWorld = glm::inverse((m_scomps.camera.proj * m_scomps.camera.view));
    glm::vec4 from = toWorld * glm::vec4(m_scomps.inputs.NDCMousePos, -1.0f, 1.0f);
    glm::vec4 to = toWorld * glm::vec4(m_scomps.inputs.NDCMousePos, 1.0f, 1.0f);
    from /= from.w; // perspective divide ("normalize" homogeneous coordinates)
    to /= to.w;
    
    if (m_scomps.inputs.actionState.at(scomp::InputAction::BRUSH_VOX_ADD)) {
        m_ctx.ddraw.addLine(from, to);
    }

    ////////////// intersection ///////////////
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    // https://www.youtube.com/watch?v=fIu_8b2n8ZM
    // https://www.youtube.com/watch?v=_P829ncXFZY
    glm::vec3 planeNormal = glm::vec3(-1, 0, 0); // Left plane normal from camera start
    glm::vec3 planePosition = glm::vec3(0);
    glm::vec3 rayDirection = glm::normalize(glm::vec3(to - from)); // Get a normal vector from the two points
    
    glm::vec3 w = planePosition - glm::vec3(from);
    float k = glm::dot(w, planeNormal) / glm::dot(rayDirection, planeNormal);
    glm::vec3 intersection = glm::vec3(from) + rayDirection * k;

    spdlog::info("Intersection point is at : {} {} {}", intersection.x, intersection.y, intersection.z);
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
