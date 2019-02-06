#version 400

#define DIR_LIGHT_COUNT 1
layout (location = 0) out vec4 FragColor;

uniform sampler2D gBuffer_Position;
uniform sampler2D gBuffer_Normal;
uniform sampler2D gBuffer_AlbedoNSpecular;

uniform vec3 DirLightAmbientColor[DIR_LIGHT_COUNT];
uniform vec3 DirLightDiffuseColor[DIR_LIGHT_COUNT];
uniform vec3 DirLightSpecularColor[DIR_LIGHT_COUNT];
uniform vec3 DirLightDirection[DIR_LIGHT_COUNT];

in VS_OUT
{
	vec2 tex_coords;
} fs_in;

/* nWorldNormal - n means that normal has to be normalized */

vec3 GetDiffuseColor(in vec3 worldPos, in vec3 nWorldNormal)
{
	vec3 resultDiffuseColor = vec3(0);

	/* POINT LIGHTS */

	/* DIRECTIONAL LIGHTS */
	for (uint dirLightIndex = 0; dirLightIndex < DIR_LIGHT_COUNT; ++dirLightIndex)
	{
		vec3 direction = -normalize(DirLightDirection[dirLightIndex]);
		float nDotD = dot(direction, nWorldNormal);
		float diffuseFactor = max(nDotD, 0.0);
		resultDiffuseColor += DirLightDiffuseColor[0] * diffuseFactor;
	}
	/* SPOT LIGHTS */

	return resultDiffuseColor;
}

vec3 GetAmbientColor()
{
	return DirLightAmbientColor[0];
}

void main()
{
	vec3 worldPos = texture(gBuffer_Position, fs_in.tex_coords).xyz;
	vec3 worldNormal = texture(gBuffer_Normal, fs_in.tex_coords).xyz;
	vec4 albedoAndSpecular = texture(gBuffer_AlbedoNSpecular, fs_in.tex_coords);

	// Lighting
	vec3 diffuseColor = GetDiffuseColor(worldPos, worldNormal);
	vec3 ambientColor = GetAmbientColor();
	vec4 totalColor = vec4(albedoAndSpecular.rgb * (diffuseColor + ambientColor), 1);

	FragColor = totalColor;
}