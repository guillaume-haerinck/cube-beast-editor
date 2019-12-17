R"(#version 300 es
precision lowp float;
layout(location = 0) out lowp vec4 color;

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
};

uniform sampler2D g_albedo;
uniform sampler2D g_normal;
uniform sampler2D g_worldPosition;

in vec2 v_texCoord;

void main() {
	vec4 albedo = texture(g_albedo, v_texCoord);
	vec3 worldPos = texture(g_worldPosition, v_texCoord).rgb;
    vec3 normal = texture(g_normal, v_texCoord).rgb;

	if (albedo.a < 0.1)
    	discard;

	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	float ambientIntensity = 1.0;
    float diffuseIntensity = 1.0;

	vec3 l_direction = vec3(-1.0, -0.5, 0.0);
	vec3 l_color = vec3(1.0, 0.0, 0.0);

	// Ambient
	float ambientStrength = 0.1f;
    vec3 ambient = l_color * ambientStrength * ambientIntensity;

    // Diffuse
    float diffuseFactor = dot(normalize(normal), -l_direction);
	if (diffuseFactor > 0.0) {
        diffuse = l_color * diffuseFactor * diffuseIntensity;
	}

	color = vec4(ambient + diffuse, 1.0f);
}

)"
