R"(#version 300 es
precision lowp float;
layout(location = 0) out vec4 color;

in vec3 v_albedo;

void main() {
	color = vec4(v_albedo, 1);
}

)"
