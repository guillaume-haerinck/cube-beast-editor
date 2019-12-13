R"(#version 300 es
layout(location = 0) out lowp vec4 g_albedo;
layout(location = 1) out lowp vec3 g_normal;
layout(location = 2) out lowp vec3 g_worldPosition;
layout(location = 3) out lowp vec4 g_id;

precision lowp float;

in lowp vec3 v_id;
in lowp vec3 v_normal;
in lowp vec3 v_worldPosition;

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
	g_normal = v_normal; // Use abs ? Because negative values are lost
	g_albedo = vec4(abs(v_normal), 1);
	g_worldPosition = v_worldPosition;
}

)"
