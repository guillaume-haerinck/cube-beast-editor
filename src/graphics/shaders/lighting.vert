R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

layout (std140) uniform perWindowChange {
    mat4 matWorld;
};

out vec2 v_texCoord;

void main() {
	v_texCoord = texCoord;
	gl_Position = matWorld * vec4(position, 1.0);
}

)"
