R"(#version 300 es
layout(location = 0) out lowp vec3 g_albedo;
layout(location = 1) out lowp vec3 g_id;

in lowp vec3 id;

void main() {
	g_id = id;
	g_albedo = vec3(1, 0.5, 0);
}

)"
