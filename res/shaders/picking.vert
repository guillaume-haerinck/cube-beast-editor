#version 300 es
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 translation;

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
	lowp vec3 cameraPos;
};

out vec3 myValue;

void main() {
	myValue = vec3(0, 0, 1);
	gl_Position = matViewProj * (vec4(position, 1.0) + vec4(translation, 1.0));
}
