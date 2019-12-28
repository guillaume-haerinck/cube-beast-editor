#pragma once

#include <array>

#include "i-system.h"
#include "context.h"
#include "scomponents/io/hovered.h"

class SelectionSystem : public ISystem {
public:
    SelectionSystem(Context& ctx, SingletonComponents& scomps);
    virtual ~SelectionSystem();

	void update() override;

private:
    Face colorToFace(unsigned char color) const;
    Face normalToFace(unsigned int normalIndex) const;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
    std::array<glm::ivec3, 6> m_planeNormals;
    std::array<glm::vec3, 6> m_planePositions;
};
