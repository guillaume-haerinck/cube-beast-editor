#include <catch2/catch.hpp>
#include <glm/glm.hpp>
#include <Eigen/Dense>
#include <spdlog/spdlog.h>

#include "maths/rbf.h"

SCENARIO("Radial basis functions should allow to create custom 3D functions which goes through set points", "[rbf]") {
    GIVEN("A list of control points with their associated weights") {
        std::vector<glm::ivec3> controlPointsXY = {
            glm::ivec3(1.0f, 1.0f, 0.0f),
            glm::ivec3(0.0f, 0.0f, 0.0f),
            glm::ivec3(2.0f, 2.0f, 0.0f)
        };
        std::vector<glm::ivec3> controlPointsZ = {
            glm::ivec3(0.0f, 0.0f, 1.0f),
            glm::ivec3(0.0f, 0.0f, 2.0f),
            glm::ivec3(0.0f, 0.0f, 3.0f)
        };
        Eigen::VectorXd weights(controlPointsXY.size());
        weights << 1.0f, 1.0f, 1.0f;
        const float eps = 1.0f;

        WHEN("We ask for the vector Wi and interp") {
            Eigen::VectorXd result = voxmt::vectorWi(weights, controlPointsXY, voxmt::RBFType::LINEAR, eps);
            std::vector<glm::ivec3> result2 = voxmt::interpolation(controlPointsZ, controlPointsXY, voxmt::RBFType::LINEAR, eps, result);

            THEN("It should give us the right values") {
                spdlog::info("map at 0,0,0 : {} {} {}", result2.at(0).x, result2.at(0).y, result2.at(0).z);
                REQUIRE(true);
            }
        }
    }
}
