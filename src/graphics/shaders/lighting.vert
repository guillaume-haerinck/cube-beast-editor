R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

void main() {
	v_texCoord = texCoord;
	gl_Position = vec4(position.x * 2.0, position.y * 2.0, position.z, 1.0);
}

)"
