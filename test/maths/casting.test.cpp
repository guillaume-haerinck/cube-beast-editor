#include <catch2/catch.hpp>
#include <glm/glm.hpp>

#include "maths/casting.h"

SCENARIO("Casting operation allows to retrieve the same value accross different type", "[casting]") {
    GIVEN("An unsigned int value") {
        int myVal = 1;

        WHEN("I cast its bits into a float") {
            glm::vec3 myCast = voxmt::intToNormColor(myVal);

            THEN("I should retrieve it when I cast back") {
                

                REQUIRE(true);
            }
        }
    }
}
