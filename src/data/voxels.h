#pragma once

#include <glm/glm.hpp>

#include "maths/sparse-set.h"

struct Voxel {
    glm::ivec3 position;
    unsigned int materialId;
};

class Voxels {
public:
    Voxels(/* args */);
    ~Voxels();

    /**
     * @brief Creates or Replace a voxel
     * @param voxel 
     */
    void set(const Voxel& voxel);

    void setMaterialIdAt(const glm::ivec3& position);

    bool exist(const glm::ivec3& position);
    Voxel at(const glm::ivec3& position);

    unsigned int getMaterialIdAt(const glm::ivec3& position);

    glm::ivec3* getPositionsData();
    unsigned int* getMaterialIdsData();

    size_t size() const;

    // TODO iterators

private:
    // cbmx::SparseSet<unsigned int> materialId;
    // cbmx::SparseSet<glm::ivec3> positions;
};
