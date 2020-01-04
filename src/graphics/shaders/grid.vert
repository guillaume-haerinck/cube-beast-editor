R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
    float debug2;
};

out vec2 v_texCoord;

void main() {
    v_texCoord = texCoord;
    gl_Position = matViewProj * vec4(position * 20.0, 1.0); // TODO use constant buffer for grid size
}

)"
