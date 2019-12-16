#pragma once

#include <glm/glm.hpp>

/**
 * @brief Geometry data for a flat-shaded cube
 * @note The 24 vertices allows us to have correct normals (perpendicular to each face)
 *       8 vertices could only provide shared normals at 135 degree angle
 */
namespace cubeData {
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    const glm::vec3 positions[] = {
        // Front v0,v1,v2,v3
        glm::vec3(1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(0, 0, 1), glm::vec3(1, 0, 1),
        // Right v0,v3,v4,v5
        glm::vec3(1, 1, 1), glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0),
        // Top v0,v5,v6,v1	
        glm::vec3(1, 1, 1), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 1), 
        // Left v1,v6,v7,v2	
        glm::vec3(0, 1, 1), glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1),  
        // Bottom v7,v4,v3,v2
        glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 1), glm::vec3(0, 0, 1), 
        // Back v4,v7,v6,v5	
        glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)
    };

    //      |/      |/
    //   ---X-------X---
    //    |/|     |/|
    // ---X-------X-|-
    //   /| |/   /| |/
    //   -|-X-----|-X---
    //    |/|     |/|
    // ---X------X---
    //   /|     /|
    const glm::vec3 normals[] = {
        glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),     // front
        glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0),     // right
        glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0),     // top
        glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), // left
        glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0),     // bottom
        glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1)      // back
    };

    // 0;1 ----- 1;1
    //  |         |
    //  |         |
    //  |         |
    // 0;0 ----- 1;0
    const glm::vec2 texCoords[] = {
        glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0),
		glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1),
		glm::vec2(1, 0), glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1),
		glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0),
		glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0), glm::vec2(0, 0)
    };

    const unsigned char indices[] = {
        0, 2, 1,   2, 0, 3,       // front
        4, 6, 5,   6, 4, 7,       // right
        8, 10,9,   10,8, 11,      // top
        12,14,13,  14,12,15,      // left
        16,18,17,  18,16,19,      // bottom
        20,22,21,  22,20,23		  // back
    };

    // Inside the cube is drawn
    const unsigned char invertIndices[] = {
        0, 1, 2,   2, 3, 0,       // front
        4, 5, 6,   6, 7, 4,       // right
        8, 9, 10,  10,11,8,       // top
        12,13,14,  14,15,12,      // left
        16,17,18,  18,19,16,      // bottom
        20,21,22,  22,23,20		  // back
    };
}

/**
 * @brief Geometry data for a square. Center of geometry is at v2, at bottom left.
 */
namespace squareData {
    //  v1-------v0
    //  |         |
    //  |         |
    //  |         |
    //  v2-------v3
    const glm::vec3 positions[] = {
        // v0,v1,v2,v3
        glm::vec3(0.5, 0.5, 0), glm::vec3(-0.5, 0.5, 0), glm::vec3(-0.5, -0.5, 0), glm::vec3(0.5, -0.5, 0)
    };

    //   X---------X
    //  /|        /|
    //   |         |
    //   |         |
    //   X---------X
    //  /         /
    const glm::vec3 normals[] = {
        glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)
    };

    // 0;1 ----- 1;1
    //  |         |
    //  |         |
    //  |         |
    // 0;0 ----- 1;0
    const glm::vec2 texCoords[] = {
        glm::vec2(1, 1), glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0)
    };

    const unsigned char indices[] = {
        0, 1, 2,   2, 3, 0
    };
}
