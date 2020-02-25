#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "i-system.h"
#include "data/io/inputs.h"
#include "data/io/hovered.h"
#include "data/graphics/materials.h"
#include "data/graphics/constant-buffers.h"
#include "data/graphics/camera.h"
#include "data/graphics/lights.h"
#include "data/graphics/meshes.h"
#include "data/graphics/pipelines.h"
#include "data/graphics/render-targets.h"
#include "data/io/viewport.h"
#include "graphics/render-command.h"

class RenderSystem : public ISystem {
public:
	RenderSystem();
	virtual ~RenderSystem();

	void update(const RenderCommand& rc,
		const ConstantBuffers& cbs, 
		const Meshes& meshes,
		const Pipelines& pips,
		RenderTargets& rtts,
		Materials& mats,
		Lights& lights,
		Camera& cam,
		const Inputs& inputs,
		const Hovered& hovered,
		const Viewport& viewport
	);

private:
	void updateCBperNiMesh_facePlane(const RenderCommand& rc, const ConstantBuffers& cbs, const Hovered& hovered);
	void updateCBperNiMesh(const RenderCommand& rc, const ConstantBuffers& cbs, glm::vec3 translation, float scale, glm::vec3 albedo);

private:
	std::vector<glm::vec3> m_tempTranslations;
	std::vector<glm::vec3> m_tempEntityIds;
	std::vector<unsigned int> m_tempMaterialIds;
};
