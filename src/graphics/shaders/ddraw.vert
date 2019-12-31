R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
    float debug2;
};

void main() {
    gl_Position = matViewProj * vec4(position, 1.0);
}

)"
