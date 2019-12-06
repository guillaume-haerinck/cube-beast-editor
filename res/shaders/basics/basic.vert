#version 300 es
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 translation;

void main() {
	gl_Position = vec4(position, 1.0) + vec4(translation, 1.0);
}
