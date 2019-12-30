R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 translation;

layout (std140) uniform perShadowPass {
    mat4 matViewProj;
};

void main() {
	gl_Position = matViewProj * vec4(position + translation, 1.0);
}

)"
