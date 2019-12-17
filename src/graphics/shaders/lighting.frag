R"(#version 300 es
precision lowp float;
layout(location = 0) out lowp vec4 color;

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
};

uniform sampler2D g_albedo;
uniform sampler2D g_normal;
uniform sampler2D g_position;

in lowp vec2 v_texCoord;

void main() {
	color = texture(g_albedo, v_texCoord);

	if (color.a < 0.1)
    	discard;
}

)"
