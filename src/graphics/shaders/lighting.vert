R"(#version 300 es
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out lowp vec2 v_texCoord;

void main() {
	v_texCoord = texCoord;
	gl_Position = vec4((position + vec3(-0.5, -0.5, 0)) * 2.0, 1.0); // TODO dirty, use a constant buffer with a model mat
}

)"
