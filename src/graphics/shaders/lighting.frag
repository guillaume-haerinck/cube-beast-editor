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
	float specularIntensity = 2.0;
    float specularAttenuation = 1.0;

	vec3 l_direction = vec3(-1.0, -0.5, 0.0);
	vec3 l_color = vec3(1.0, 0.0, 0.0);
	float l_intensity = 1.0;

	// Ambient
	float ambientStrength = 0.1;
    vec3 ambient = l_color * ambientStrength * ambientIntensity;

    // Diffuse
    float diffuseFactor = dot(normalize(normal), -l_direction);
	if (diffuseFactor > 0.0) {
        diffuse = l_color * diffuseFactor * diffuseIntensity;

		// Specular
		vec3 vertexToCamera = normalize(cameraPos - worldPos);
		vec3 lightReflect = normalize(reflect(l_direction, normal));
		float specularFactor = dot(vertexToCamera, lightReflect);

		if (specularFactor > 0.0) {
			specularFactor = pow(specularFactor, specularAttenuation);
            specular = l_color * specularFactor * specularIntensity;
        }
	}

	color = vec4(ambient + diffuse + specular, 1.0f) * l_intensity;
}

)"
