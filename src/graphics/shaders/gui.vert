R"(#version 300 es
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

layout (std140) uniform perFrame {
    lowp mat4 matViewProj;
	lowp vec3 cameraPos;
};

out lowp vec2 v_texCoord;

void main() {
    v_texCoord = texCoord;
    gl_Position = matViewProj * vec4(position, 1.0);
}

)"
