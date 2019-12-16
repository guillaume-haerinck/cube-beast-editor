#pragma once

#include <array>

#include "i-system.h"
#include "context.h"
#include "scomponents/io/selection.h"

class SelectionSystem : public ISystem {
public:
    SelectionSystem(Context& ctx, SingletonComponents& scomps);
    virtual ~SelectionSystem();

	void update() override;

private:
    scomp::Face colorToFace(unsigned char color) const;
    scomp::Face normalToFace(const glm::vec3& normal) const;

private:
    Context& m_ctx;
    SingletonComponents& m_scomps;
    std::array<glm::vec3, 6> m_planeNormals;
    std::array<glm::vec3, 6> m_planePositions;
};
