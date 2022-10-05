uniform vec3 viewPos;

///////////////////////////////
// Lighting
///////////////////////////////

struct Light {
	vec3 direction;
	vec3 color;
	float intensity;
	mat4 lightSpaceMatrix;
};

struct PositionalLight {
	Light base;
	vec3 position;
	vec3 attenuationCLQ;
	float spotAngle;
	float outerSpotAngle;
};

struct Lighting {
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;
};

uniform Light directionalLight;
uniform int lightCount;
uniform PositionalLight lights[16];

uniform sampler2DShadow directionalLightShadowMap;
uniform sampler2DShadow spotLightShadowMap;
uniform samplerCube skybox;

float OffsetLookUp(sampler2DShadow map, vec3 loc, vec2 offset)
{
	vec2 textureMapScale = 1.0 / vec2(textureSize(map, 0));
	return texture(map, vec3(loc.xy + offset * textureMapScale, loc.z)); 
}

float CalculateShadow(sampler2DShadow shadowMap, vec4 fragPosLightSpace, vec3 normal, vec3 bias)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
	projCoords += bias;

	vec2 offset = fract(projCoords.xy * 0.5); 
	if (offset.y > 1.1)
		offset.y = 0.0;

	float shadowCoeff = 0.0;
	float shadowSamples = sqrt(36);
	float halfSS = shadowSamples * 0.5;
	int takenSamples = 0;

	for (float x =-halfSS; x < halfSS; x++)
	{
		for (float y = -halfSS; y < halfSS; y++)
		{
			vec2 localOffset = vec2(x, y) * 0.75;
			shadowCoeff += OffsetLookUp(shadowMap, projCoords, offset + localOffset);
			takenSamples++;
		}
	}

	shadowCoeff /= takenSamples;
	float shadow = 1.0 - shadowCoeff;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
} 

vec3 EvaluateLighting(Lighting lighting) {
	return lighting.ambient + (lighting.diffuse + lighting.specular);
}

vec3 GetAmbientLighting() {
	return vec3(0.00, 0.02, 0.03);
}

Lighting CalculateLightByDirection(Light light, vec3 N, vec3 L, vec3 V, float shininess)
{
	vec3 ambient = GetAmbientLighting();

	float diffuseFactor = max(dot(L, N), 0.0);
	vec3 diffuseColor = diffuseFactor * light.color * light.intensity;

	vec3 halfwayDir = normalize(L + V);
	vec3 specularColor = vec3(0);
	if (diffuseFactor > 0.0)
	{
		float specularFactor = pow(max(dot(N, halfwayDir), 0.0), shininess);
		if (specularFactor > 0.0)
		{
			specularColor = specularFactor * light.color * light.intensity;
		}
	}

	return Lighting(diffuseColor, specularColor, ambient);
}

Lighting CalculateDirectionalLight(Light light, vec3 N, vec3 V, float shininess) {
	vec3 lightDir = normalize(-light.direction);
	
	return CalculateLightByDirection(light, N, lightDir, V, shininess);
}

Lighting CalculateLight(PositionalLight light, vec3 N, vec3 V, vec3 fragPos, float shininess)
{
	vec3 L = light.position - fragPos;

	float distance = length(L);
	L = normalize(L);

	Lighting lighting = CalculateLightByDirection(light.base, N, L, V, shininess);
	float attenuation = light.attenuationCLQ.x * distance * distance + 
						light.attenuationCLQ.y * distance +
						light.attenuationCLQ.z;

	lighting.diffuse /= attenuation,
	lighting.specular /= attenuation;
	lighting.ambient /= attenuation;

	if (light.spotAngle >= 0.0)
	{
		float theta = dot(L, normalize(-light.base.direction));
		float epsilon = light.spotAngle - light.outerSpotAngle;
		float intensity = clamp((theta - light.outerSpotAngle) / epsilon, 0.0, 1.0); 

		lighting.diffuse *= intensity;
		lighting.specular *= intensity;
	}

	return lighting;
}

Lighting CalculateLighting(vec3 normal, vec3 viewDir, vec3 fragPos, float shininess, float roughness) {
	Lighting result;

	Lighting dirLight = CalculateDirectionalLight(directionalLight, normal, viewDir, shininess);
	result.diffuse += dirLight.diffuse;
	result.specular += dirLight.specular;
	result.ambient += dirLight.ambient;

	vec3 bias = vec3(vec2(0), -max(0.001 * (1.0 - dot(normal, -directionalLight.direction)), 0.001)); 
	float dirLightShadow = CalculateShadow(directionalLightShadowMap,
		directionalLight.lightSpaceMatrix * vec4(fragPos, 1.0), normal, bias);
	result.diffuse *= 1.0 - dirLightShadow;
	result.specular *= 1.0 - dirLightShadow;

	for (int i = 0; i < lightCount; i++) {
		Lighting lighting = CalculateLight(lights[i], normal, viewDir, fragPos, shininess);

		result.diffuse += lighting.diffuse;
		result.specular += lighting.specular;
		result.ambient += lighting.ambient;
	}

	return result;
}