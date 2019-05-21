#version 400

#define MAX_DIR_LIGHT_COUNT 5
#define MAX_POINT_LIGHT_COUNT 1
#define SHADOWMAP_BIAS 0.005
#define PCF_SAMPLES 2
layout (location = 0) out vec4 FragColor;

uniform sampler2D gBuffer_Position;
uniform sampler2D gBuffer_Normal;
uniform sampler2D gBuffer_AlbedoNSpecular;
uniform sampler2D DirLightShadowMaps[MAX_DIR_LIGHT_COUNT];

uniform vec3 DirLightAmbientColor[MAX_DIR_LIGHT_COUNT];
uniform vec3 DirLightDiffuseColor[MAX_DIR_LIGHT_COUNT];
uniform vec3 DirLightSpecularColor[MAX_DIR_LIGHT_COUNT];
uniform vec3 DirLightDirection[MAX_DIR_LIGHT_COUNT];
uniform mat4 DirLightShadowMatrices[MAX_DIR_LIGHT_COUNT];
uniform vec4 DirLightShadowAtlasOffset[MAX_DIR_LIGHT_COUNT];
uniform uint DirLightCount;
uniform int DirLightShadowMapCount;

uniform uint PointLightCount;
uniform vec3 PointLightDiffuseColor[MAX_POINT_LIGHT_COUNT];
uniform vec3 PointLightSpecularColor[MAX_POINT_LIGHT_COUNT];
uniform vec3 PointLightPosition[MAX_POINT_LIGHT_COUNT];
uniform vec3 PointLightAttenuation[MAX_POINT_LIGHT_COUNT];

in VS_OUT
{
	vec2 tex_coords;
} fs_in;

vec2 GetShadowTexCoords(in vec2 texCoords, in vec4 atlasOffset)
{
	vec2 texCoordsInAtlas;
	texCoordsInAtlas = (texCoords * atlasOffset.zw) + atlasOffset.xy;
	return texCoordsInAtlas;
}

float CalcLitFactor(in sampler2D shadowmap, in vec2 shadowmapSize, in vec3 shadowTexCoord)
{
	float resultShadow = 0.0;
    float actualDepth = shadowTexCoord.z - SHADOWMAP_BIAS;

    float SumDepth = 0.0;
    vec2 texelSize = 1.0 / shadowmapSize;
    int countSamples = 0;
    for (int x = -PCF_SAMPLES; x <= PCF_SAMPLES; x++)
    {
       for (int y = -PCF_SAMPLES; y <= PCF_SAMPLES; y++)
       {
			vec2 offset = vec2(float(x) * texelSize.x, float(y) * texelSize.y);

            float pcfDepth = texture(shadowmap, shadowTexCoord.xy + offset).r;
            resultShadow += actualDepth > pcfDepth ? 1.0 : 0.0;
            countSamples++;
       }
    }
    resultShadow /= countSamples;
    resultShadow = 1 - resultShadow;
	return resultShadow;
}

/* nWorldNormal - n means that normal has to be normalized */

vec3 GetDiffuseColor(in vec3 worldPos, in vec3 nWorldNormal)
{
	vec3 resultDiffuseColor = vec3(0);

	/* POINT LIGHTS */
	for (uint pointLightIndex = 0; pointLightIndex < PointLightCount; ++pointLightIndex)
	{
		vec3 nToLightVec = normalize(PointLightPosition[pointLightIndex] - worldPos);
		float nDotP = dot(nToLightVec, nWorldNormal);
		float diffuseFactor = max(nDotP, 0.0);
		resultDiffuseColor += PointLightDiffuseColor[pointLightIndex] * diffuseFactor;
	}

	/* DIRECTIONAL LIGHTS */
	for (uint dirLightIndex = 0; dirLightIndex < DirLightCount; ++dirLightIndex)
	{
		vec3 direction = -normalize(DirLightDirection[dirLightIndex]);
		float nDotD = dot(direction, nWorldNormal);
		float diffuseFactor = max(nDotD, 0.0);
		float litFactor = 1.0f;

		if (DirLightShadowMapCount > dirLightIndex)
		{
			// Calculating shadow
			// Common
			vec4 atlasOffset = DirLightShadowAtlasOffset[dirLightIndex];
			mat4 shadowMatrix = DirLightShadowMatrices[dirLightIndex];
			//

			vec4 shadowProjectedPosition = (shadowMatrix * vec4(worldPos, 1.0));
			vec3 shadowFragCoords = shadowProjectedPosition.xyz / shadowProjectedPosition.w;
			vec2 shadowCoordinates = GetShadowTexCoords(shadowFragCoords.xy, atlasOffset);
			vec3 shadowCoordinatesAndDepth = vec3(shadowCoordinates, shadowFragCoords.z);

		 	vec2 shadowmapAtlasSize = textureSize(DirLightShadowMaps[dirLightIndex], 0);
		    litFactor = CalcLitFactor(DirLightShadowMaps[dirLightIndex], shadowmapAtlasSize, shadowCoordinatesAndDepth);
		}

		resultDiffuseColor += DirLightDiffuseColor[dirLightIndex] * diffuseFactor * litFactor;
	}
	/* SPOT LIGHTS */
	// TODO: SPOT LIGHTS

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