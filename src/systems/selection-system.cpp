#include "selection-system.h"

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <profiling/instrumentor.h>

#include "maths/intersection.h"
#include "maths/casting.h"
#include "components/physics/transform.h"
#include "graphics/gl-exception.h"

SelectionSystem::SelectionSystem(Context& ctx, SingletonComponents& scomps) 
    : m_ctx(ctx), m_scomps(scomps)
{
    // TODO set 19.5 to scene size
    m_planePositions = {
        glm::vec3(0),
        glm::vec3(0),
        glm::vec3(0),
        glm::vec3(19.5, 0, 0),
        glm::vec3(0, 19.5, 0),
        glm::vec3(0, 0, 19.5)
    };

    m_planeNormals = {
        glm::ivec3(-1, 0, 0),     // right
        glm::ivec3(0, -1, 0),     // top 
        glm::ivec3(0, 0, -1),     // back
        glm::ivec3(1, 0, 0),      // left
        glm::ivec3(0, 1, 0),      // bottom
        glm::ivec3(0, 0, 1)       // front
    };

}

SelectionSystem::~SelectionSystem() {}

void SelectionSystem::update() {
    PROFILE_SCOPE("SelectionSystem update");

    m_ctx.rcommand.bindRenderTarget(m_scomps.renderTargets.at(RenderTargetIndex::RTT_GEOMETRY)); // Needed for wasm
    unsigned char* pixel;
    {
        OGL_SCOPE("Read framebuffer for selection");
        pixel = m_ctx.rcommand.readPixelBuffer(m_scomps.renderTargets.at(RenderTargetIndex::RTT_GEOMETRY).pixelBuffer);
    }

    // FIXME intersection point take value "-+4.76837e-07" instead of 0.0 sometimes which causes flicker
    m_scomps.hovered.m_exist = false;
    const met::entity hoveredCube = voxmt::colorToInt(pixel[0], pixel[1], pixel[2]);

    // Check existing cubes with framebuffer
    if (hoveredCube != met::null) {
        m_scomps.hovered.m_exist = true;
        m_scomps.hovered.m_isCube = true;
        m_scomps.hovered.m_face = colorToFace(pixel[3]);
        const comp::Transform trans = m_ctx.registry.get<comp::Transform>(hoveredCube);
        m_scomps.hovered.m_position = trans.position;
        m_scomps.hovered.m_id = hoveredCube;
        
    } else {
        PROFILE_SCOPE("Raycasting");
        m_scomps.hovered.m_id = met::null;

        // Check grid with raycast
        const glm::mat4 toWorld = glm::inverse((m_scomps.camera.proj() * m_scomps.camera.view()));
        glm::vec4 from = toWorld * glm::vec4(m_scomps.inputs.ndcMousePos(), -1.0f, 1.0f);
        glm::vec4 to = toWorld * glm::vec4(m_scomps.inputs.ndcMousePos(), 1.0f, 1.0f);
        from /= from.w;
        to /= to.w;

        glm::vec3 intersectionPoint;
        for (unsigned int i = 0; i < m_planeNormals.size(); i++) {
            if (voxmt::doesLineIntersectPlane(m_planeNormals.at(i), m_planePositions.at(i), from, to, intersectionPoint)) {
                const bool isInsideMin = intersectionPoint.x >= 0.0f && intersectionPoint.y >= 0.0f && intersectionPoint.z >= 0.0f;
                const bool isInsideMax = intersectionPoint.x <= 20.0f && intersectionPoint.y <= 20.0f && intersectionPoint.z <= 20.0f; // TODO use scomp for object size

                if (isInsideMin && isInsideMax) {
                    m_scomps.hovered.m_exist = true;
                    m_scomps.hovered.m_isCube = false;
                    m_scomps.hovered.m_face = normalToFace(i);
                    m_scomps.hovered.m_position = voxmt::roundUpFloat3(intersectionPoint);
                    break;
                }
            }
        }
    }
}

Face SelectionSystem::colorToFace(unsigned char color) const {
    switch (color) {
        case 0: return Face::NONE;
        case 1: return Face::BACK;
        case 2: return Face::RIGHT;
        case 3: return Face::TOP;
        case 4: return Face::FRONT;
        case 5: return Face::LEFT;
        case 6: return Face::BOTTOM;
        default:
            debug_break();
            assert(false && "Unknown face picked");
            return Face::NONE;
    }
}

Face SelectionSystem::normalToFace(unsigned int normalIndex) const {
    switch (normalIndex) {
        case 0: return Face::RIGHT;
        case 1: return Face::TOP;
        case 2: return Face::BACK;
        case 3: return Face::LEFT;
        case 4: return Face::BOTTOM;
        case 5: return Face::FRONT;
        default:
            debug_break();
            assert(false && "Unknown face picked");
            return Face::NONE;
    }
}
