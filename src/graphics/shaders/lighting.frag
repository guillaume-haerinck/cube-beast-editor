R"(#version 300 es
precision lowp float;
layout(location = 0) out lowp vec4 color;

struct DirectionalLight {
	vec3 direction;
	float intensity;
	vec3 color;
};

struct PointLight {
	vec3 position;
	float intensity;
	vec3 color;
	float attenuationRadius;
};

struct SpotLight {
	vec3 direction;
	float intensity;
	vec3 position;
	float spotAngle;
	vec3 color;
	float attenuationRadius;
};

layout (std140) uniform perFrame {
    mat4 matViewProj;
	vec3 cameraPos;
};

layout (std140) uniform perLightChange {
    DirectionalLight dirLights[MAX_COUNT_DIR_LIGHTS];
	// PointLight pointLights[MAX_COUNT_POINT_LIGHTS];
	// SpotLight spotLights[MAX_COUNT_SPOT_LIGHTS];
};

uniform sampler2D g_albedo;
uniform sampler2D g_normal;

in vec2 v_texCoord;

void main() {
	vec4 albedo = texture(g_albedo, v_texCoord);
    vec3 normal = texture(g_normal, v_texCoord).rgb;

	if (albedo.a < 0.1)
    	discard;

	// Ambient
	float ambientStrength = 0.4f;
    vec3 ambient = dirLights[0].color * ambientStrength;

    // Diffuse
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
    float diffuseFactor = dot(normalize(normal), -dirLights[0].direction);
	if (diffuseFactor > 0.0) {
        diffuse = dirLights[0].color * diffuseFactor;
	}

	color = albedo * vec4(ambient + diffuse, 1.0f) * dirLights[0].intensity;
}

)"
