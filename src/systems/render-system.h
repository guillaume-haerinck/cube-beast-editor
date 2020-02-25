#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "i-system.h"
#include "scomponents/io/inputs.h"
#include "scomponents/io/hovered.h"
#include "scomponents/graphics/materials.h"
#include "scomponents/graphics/constant-buffers.h"
#include "scomponents/graphics/camera.h"
#include "scomponents/graphics/lights.h"
#include "scomponents/graphics/meshes.h"
#include "scomponents/graphics/pipelines.h"
#include "scomponents/graphics/render-targets.h"
#include "scomponents/io/viewport.h"
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
