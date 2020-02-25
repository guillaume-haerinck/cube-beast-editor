#pragma once

#include <array>

#include "data/io/hovered.h"
#include "data/io/inputs.h"
#include "data/graphics/camera.h"
#include "data/graphics/render-targets.h"
#include "data/io/hovered.h"
#include "graphics/render-command.h"
#include "systems/i-system.h"

class SelectionSystem : public ISystem {
public:
    SelectionSystem();
    virtual ~SelectionSystem();

	void update(const RenderCommand& rc,
        const RenderTargets& rtt,
        const Camera& cam,
        const Inputs& inputs,
        Hovered& hovered
    );

private:
    Face colorToFace(unsigned char color) const;
    Face normalToFace(unsigned int normalIndex) const;

private:
    std::array<glm::ivec3, 6> m_planeNormals;
    std::array<glm::vec3, 6> m_planePositions;
};
