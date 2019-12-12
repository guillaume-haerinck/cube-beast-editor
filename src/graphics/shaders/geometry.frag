R"(#version 300 es
layout(location = 0) out lowp vec4 g_albedo;
layout(location = 1) out lowp vec3 g_normal;
layout(location = 2) out lowp vec3 g_id;

in lowp vec3 v_id;
in lowp vec3 v_normal;

void main() {
	g_id = v_id;
	g_normal = v_normal;
	g_albedo = vec4(v_normal, 1);
}

)"
