R"(#version 300 es
precision lowp float;
layout(location = 0) out lowp vec4 color;

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
};

layout (std140) uniform perLightChange {
    vec3 l_color;
	float l_intensity;
	vec3 l_position;
	float l_spotAngle;
	vec3 l_direction;
	float l_attenuationRadius;
};

uniform sampler2D g_albedo;
in lowp vec2 v_texCoord;

void main() {
	color = texture(g_albedo, v_texCoord);

	if (color.a < 0.1)
    	discard;
}

)"
