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
	float debug2;
};

layout (std140) uniform perLightChange {
    DirectionalLight dirLights[MAX_COUNT_DIR_LIGHTS];
	// PointLight pointLights[MAX_COUNT_POINT_LIGHTS];
	// SpotLight spotLights[MAX_COUNT_SPOT_LIGHTS];
};

uniform sampler2D g_albedo;
uniform sampler2D g_normal;
uniform sampler2D g_lightSpacePosition;
uniform sampler2D shadowMap;

in vec2 v_texCoord;

void main() {
	vec4 albedo = texture(g_albedo, v_texCoord);
    vec3 normal = texture(g_normal, v_texCoord).rgb;
	vec4 lightSpacePos = texture(g_lightSpacePosition, v_texCoord);

	if (albedo.a < 0.1)
    	discard;

	// Shadows
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	shadow = 1.0 - shadow;
	
	// Ambient
	float ambientStrength = 0.15;
    vec3 ambient = dirLights[0].color * albedo.rgb * ambientStrength;

    // Diffuse
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
    float diffuseFactor = dot(normalize(normal), -dirLights[0].direction);
	if (diffuseFactor > 0.0) {
        diffuse = dirLights[0].color * diffuseFactor;
	}

	vec3 lighting = (ambient + shadow) * diffuse * albedo.rgb;
	color = vec4(lighting, 1.0);
}

)"
