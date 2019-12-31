R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;

layout (std140) uniform perNiMesh {
    mat4 matWorld;
    vec3 albedo;
    float debug1;
};

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
    float debug2;
};

out vec3 v_albedo;

void main() {
    v_albedo = albedo;
    gl_Position = matViewProj * matWorld * vec4(position, 1.0);
}

)"
