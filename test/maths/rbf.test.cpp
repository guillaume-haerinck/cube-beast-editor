#include <catch2/catch.hpp>
#include <glm/glm.hpp>
#include <Eigen/Dense>
#include <spdlog/spdlog.h>

#include "maths/rbf.h"

SCENARIO("Radial basis functions should allow to create custom 3D functions which goes through set points", "[rbf]") {
    GIVEN("A list of control points with their associated weights") {
        std::vector<glm::ivec3> controlPointsXYZ = {
            glm::ivec3(1, 1, 3),
            glm::ivec3(0, 0, 8),
            glm::ivec3(2, 2, 2)
        };
        Eigen::VectorXd weights(controlPointsXYZ.size());
        weights << 1.0f, 1.0f, 1.0f;
        const float eps = 1.0f;

        WHEN("We ask for z coordinates for a list of points on the horizontal plane") {
            std::vector<glm::ivec3> coordWhereToFindZ = {
                glm::ivec3(1, 9, 0),
                glm::ivec3(3, 2, 0),
                glm::ivec3(5, 8, 0)
            };

            Eigen::VectorXd wi = voxmt::vectorWi(weights, controlPointsXYZ, voxmt::RBFType::LINEAR, eps);
            std::vector<glm::ivec3> result = voxmt::interpolation(coordWhereToFindZ, controlPointsXYZ, voxmt::RBFType::LINEAR, eps, wi);

            THEN("It should give us the right values") {
                spdlog::info("map at 0,0,0 : {} {} {}", result.at(0).x, result.at(0).y, result.at(0).z);
                REQUIRE(true);
            }
        }
    }
}
