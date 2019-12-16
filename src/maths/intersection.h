#pragma once

#include <glm/glm.hpp>

namespace voxmt {
    /**
     * @brief Say wether a line intersects a plane. Gives the point of intersection in case of intersection.
     * 
     * @param planeNormal 
     * @param planePosition 
     * @param lineStart 
     * @param lineEnd 
     * @param intersectionPoint 
     * @return true 
     * @return false 
     */
    bool doesLineIntersectPlane(const glm::vec3& planeNormal, const glm::vec3& planePosition, const glm::vec3& lineStart, const glm::vec3& lineEnd, glm::vec3& intersectionPoint);
}
