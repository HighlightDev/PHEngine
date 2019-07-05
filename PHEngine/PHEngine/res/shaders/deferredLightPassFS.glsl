#version 400

#define PCF_SAMPLES 2
#define MAX_DIR_LIGHT_COUNT 5
#define MAX_POINT_LIGHT_COUNT 1
#define SHADOWMAP_BIAS_DIR_LIGHT 0.005
#define SHADOWMAP_BIAS_POINT_LIGHT 0.05
#define PCF_SAMPLES_DIR_LIGHT 2
#define PCF_SAMPLES_POINT_LIGHT 4

const float INV_COUNT_PCF_DIR_LIGHT_SAMPLES = 1.0 / (((PCF_SAMPLES_DIR_LIGHT * 2) + 1) * ((PCF_SAMPLES_DIR_LIGHT * 2) + 1));
const float INV_COUNT_PCF_POINT_LIGHT_SAMPLES = 1.0 / (PCF_SAMPLES_POINT_LIGHT * PCF_SAMPLES_POINT_LIGHT * PCF_SAMPLES_POINT_LIGHT);

layout (location = 0) out vec4 FragColor;

uniform sampler2D gBuffer_Position;
uniform sampler2D gBuffer_Normal;
uniform sampler2D gBuffer_AlbedoNSpecular;
uniform sampler2D DirLightShadowMaps[MAX_DIR_LIGHT_COUNT];
uniform samplerCube PointLightShadowMaps[MAX_POINT_LIGHT_COUNT];

uniform vec3 DirLightAmbientColor[MAX_DIR_LIGHT_COUNT];
uniform vec3 DirLightDiffuseColor[MAX_DIR_LIGHT_COUNT];
uniform vec3 DirLightSpecularColor[MAX_DIR_LIGHT_COUNT];
uniform vec3 DirLightDirection[MAX_DIR_LIGHT_COUNT];
uniform mat4 DirLightShadowMatrices[MAX_DIR_LIGHT_COUNT];
uniform vec4 DirLightShadowAtlasOffset[MAX_DIR_LIGHT_COUNT];
uniform uint DirLightCount;
uniform int DirLightShadowMapCount;

uniform uint PointLightCount;
uniform int PointLightShadowMapCount;
uniform vec3 PointLightDiffuseColor[MAX_POINT_LIGHT_COUNT];
uniform vec3 PointLightSpecularColor[MAX_POINT_LIGHT_COUNT];
uniform vec3 PointLightAttenuation[MAX_POINT_LIGHT_COUNT];
uniform float PointLightShadowProjectionFarPlane[MAX_POINT_LIGHT_COUNT];
uniform vec3 PointLightPositionWorld[MAX_POINT_LIGHT_COUNT];

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

float CalcLitFactorCubemap(in samplerCube shadowmap, in vec3 worldPos, in vec3 pointLightWorldPos, in float shadowmapProjectionfarPlane)
{
	float resultLit = 0.0f;

	vec3 LightToFragVec = worldPos - pointLightWorldPos;
	float actualDepth = length(LightToFragVec);

	float shadow  = 0.0;
	float bias    = 0.05;
	float samples = 4.0;
	float offset  = 0.1;

	for (float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for (float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for (float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float shadowmapDepth = texture(shadowmap, LightToFragVec + vec3(x, y, z)).r; // depth is in range [0 ; 1]
				shadowmapDepth *= shadowmapProjectionfarPlane; // now depth is linear in world space in range [0 ; Far Plane]
				shadow += actualDepth - SHADOWMAP_BIAS_POINT_LIGHT > shadowmapDepth ? 1.0f : 0.0f;
			}
		}
	}

	shadow *= INV_COUNT_PCF_POINT_LIGHT_SAMPLES;
	resultLit = 1 - shadow;
	return resultLit;
}

float CalcLitFactorTexture2D(in sampler2D shadowmap, in vec2 shadowmapSize, in vec3 shadowTexCoord)
{
	float resultLit = 0.0;
    float actualDepth = shadowTexCoord.z - SHADOWMAP_BIAS_DIR_LIGHT;

    float SumDepth = 0.0;
    vec2 texelSize = 1.0 / shadowmapSize;

    for (int x = -PCF_SAMPLES; x <= PCF_SAMPLES; x++)
    {
       for (int y = -PCF_SAMPLES; y <= PCF_SAMPLES; y++)
       {
			vec2 offset = vec2(float(x) * texelSize.x, float(y) * texelSize.y);

            float pcfDepth = texture(shadowmap, shadowTexCoord.xy + offset).r;
            resultLit += actualDepth > pcfDepth ? 1.0 : 0.0;
       }
    }

    resultLit *= INV_COUNT_PCF_DIR_LIGHT_SAMPLES;
    resultLit = 1 - resultLit;
	return resultLit;
}

/* nWorldNormal - n means that normal has to be normalized */

vec3 GetDiffuseColor(in vec3 worldPos, in vec3 nWorldNormal)
{
	vec3 resultDiffuseColor = vec3(0);

	/* POINT LIGHTS */
	for (uint pointLightIndex = 0; pointLightIndex < PointLightCount; ++pointLightIndex)
	{
		vec3 pointLightPositionWorld = PointLightPositionWorld[pointLightIndex];
		vec3 nToLightVec = normalize(pointLightPositionWorld - worldPos);
		float nDotP = dot(nToLightVec, nWorldNormal);
		float diffuseFactor = max(nDotP, 0.0);
		float litFactor = 1.0f;

		// Calculating shadow
		if (PointLightShadowMapCount > pointLightIndex)
		{
			litFactor = CalcLitFactorCubemap(PointLightShadowMaps[pointLightIndex], worldPos, pointLightPositionWorld, PointLightShadowProjectionFarPlane[pointLightIndex]);
		}

		resultDiffuseColor += PointLightDiffuseColor[pointLightIndex] * diffuseFactor * litFactor;
	}

	/* DIRECTIONAL LIGHTS */
	for (uint dirLightIndex = 0; dirLightIndex < DirLightCount; ++dirLightIndex)
	{
		vec3 direction = -normalize(DirLightDirection[dirLightIndex]);
		float nDotD = dot(direction, nWorldNormal);
		float diffuseFactor = max(nDotD, 0.0);
		float litFactor = 1.0f;

		// Calculating shadow
		if (DirLightShadowMapCount > dirLightIndex)
		{
			// Common
			vec4 atlasOffset = DirLightShadowAtlasOffset[dirLightIndex];
			mat4 shadowMatrix = DirLightShadowMatrices[dirLightIndex];
			//

			vec4 shadowProjectedPosition = (shadowMatrix * vec4(worldPos, 1.0));
			vec3 shadowFragCoords = shadowProjectedPosition.xyz / shadowProjectedPosition.w;
			vec2 shadowCoordinates = GetShadowTexCoords(shadowFragCoords.xy, atlasOffset);
			vec3 shadowCoordinatesAndDepth = vec3(shadowCoordinates, shadowFragCoords.z);

		 	vec2 shadowmapAtlasSize = textureSize(DirLightShadowMaps[dirLightIndex], 0);
		    litFactor = CalcLitFactorTexture2D(DirLightShadowMaps[dirLightIndex], shadowmapAtlasSize, shadowCoordinatesAndDepth);
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