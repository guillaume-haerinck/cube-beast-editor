R"(#version 300 es
precision lowp float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 translation;
layout(location = 3) in vec3 entityId;
layout(location = 4) in uint materialId;

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
};

out vec3 v_id;
out vec3 v_normal;
out vec3 v_worldPosition;
flat out uint v_materialId;

void main() {
	v_id = entityId;
	v_normal = normal;
	v_worldPosition = position + translation;
	v_materialId = materialId;
	gl_Position = matViewProj * vec4(position + translation, 1.0);
}

)"
