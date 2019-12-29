R"(#version 300 es
precision lowp float;
layout(location = 0) out vec4 g_albedo;
layout(location = 1) out vec4 g_normal;
layout(location = 2) out vec4 g_worldPosition;
layout(location = 3) out vec4 g_id;

layout (std140) uniform perMaterialChange {
	vec3 albedo[5];
	float emissiveFactor[5];
};

in vec3 v_id;
in vec3 v_normal;
in vec3 v_worldPosition;
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
	g_albedo = vec4(albedo[v_materialId], 1.0);
	g_worldPosition = vec4(v_worldPosition, 0.0);
}

)"
