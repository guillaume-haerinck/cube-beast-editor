R"(#version 300 es
precision lowp float;
layout(location = 0) out vec4 g_albedo;
layout(location = 1) out vec4 g_normal;
layout(location = 2) out vec4 g_lightSpacePosition;
layout(location = 3) out vec4 g_id;

struct Material {
	vec3 albedo;
	float emissiveFactor;
};

layout (std140) uniform perMaterialChange {
	Material materials[MAX_COUNT_MATERIALS];
};

in vec3 v_id;
in vec3 v_normal;
in vec4 v_lightSpacePosition;
flat in uint v_materialId;

float getFaceNumber(vec3 normal) {
	if (normal.z >= 0.9)
		return 0.00392; // 1
	if (normal.x >= 0.9)
		return 0.00784; // 2
	if (normal.y >= 0.9)
		return 0.01176; // 3
	if (normal.z <= -0.9)
		return 0.01569; // 4
	if (normal.x <= -0.9)
		return 0.01961; // 5
	if (normal.y <= -0.9)
		return 0.02353; // 6

	return 0.0;
}

void main() {
	g_id = vec4(v_id, getFaceNumber(v_normal));
	g_normal = vec4(v_normal, 1.0);
	g_albedo = vec4(materials[v_materialId].albedo, 1.0);
	g_lightSpacePosition = v_lightSpacePosition;
}

)"
