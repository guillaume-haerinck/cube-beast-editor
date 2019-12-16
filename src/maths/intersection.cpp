#include "intersection.h"

namespace voxmt {
    bool doesLineIntersectPlane(const glm::vec3& planeNormal, const glm::vec3& planePosition, const glm::vec3& lineStart, const glm::vec3& lineEnd, glm::vec3& intersectionPoint) {
        const glm::vec3 v = lineEnd - lineStart;
        const glm::vec3 w = planePosition - lineStart;

        if (glm::dot(v, planeNormal) <= 0.0f) {
            return false;
        }
        
        const float k = glm::dot(w, planeNormal) / glm::dot(v, planeNormal);
        intersectionPoint = lineStart + k * v;

        return k >= 0.0f && k <= 1.0f;
    }
}
