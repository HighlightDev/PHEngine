#version 400

#define SHADING_MODEL_PBR
#define MAX_DIR_LIGHT_COUNT 5
#define MAX_POINT_LIGHT_COUNT 50
#define SHADOWMAP_BIAS_DIR_LIGHT 0.005
#define SHADOWMAP_BIAS_POINT_LIGHT 0.05
#define PCF_SAMPLES_DIR_LIGHT 2
#define PCF_SAMPLES_POINT_LIGHT 4
#define MAX_POINT_LIGHT_SHADOW_MAP_COUNT 4
#define MAX_DIR_LIGHT_SHADOW_MAP_COUNT 4

const float INV_COUNT_PCF_DIR_LIGHT_SAMPLES = 1.0 / (((PCF_SAMPLES_DIR_LIGHT * 2) + 1) * ((PCF_SAMPLES_DIR_LIGHT * 2) + 1));
const float INV_COUNT_PCF_POINT_LIGHT_SAMPLES = 1.0 / (PCF_SAMPLES_POINT_LIGHT * PCF_SAMPLES_POINT_LIGHT * PCF_SAMPLES_POINT_LIGHT);

layout (location = 0) out vec4 FragColor;

uniform vec3 CameraWorldPosition;

uniform sampler2D gBuffer_Position;
uniform sampler2D gBuffer_Normal;
uniform sampler2D gBuffer_AlbedoNSpecular;
uniform sampler2D DirLightShadowMaps[MAX_DIR_LIGHT_SHADOW_MAP_COUNT];
uniform samplerCube PointLightShadowMaps[MAX_POINT_LIGHT_SHADOW_MAP_COUNT];

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

#ifdef SHADING_MODEL_PBR

	float Metallic = 0.8;
	float Roughness = 0.5;
	const vec4 albedoCOLOR = vec4(1.0, 0.0, 0.0, 1.0);
	const float Epsilon = 0.00001;
	uniform float ao;

	const float PI = 3.14159265359;

	float DistributionGGX(vec3 N, vec3 H, float roughness)
	{
	    float a      = roughness * roughness;
	    float a2     = a * a;
	    float NdotH  = max(dot(N, H), 0.0);
	    float NdotH2 = NdotH * NdotH;

	    float num   = a2;
	    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	    denom = PI * denom * denom;

	    return num / denom;
	}

	float GeometrySchlickGGX(float NdotV, float roughness)
	{
	    float r = (roughness + 1.0);
	    float k = (r * r) * 0.125; // 1 / 8

	    float num   = NdotV;
	    float denom = NdotV * (1.0 - k) + k;

	    return num / denom;
	}

	float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
	{
	    float NdotV = max(dot(N, V), 0.0);
	    float NdotL = max(dot(N, L), 0.0);
	    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
	    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

	    return ggx1 * ggx2;
	}

//	vec3 fresnelSchlick(float cosTheta, vec3 F0)
//	{
//		return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
//	}


float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

	// Shlick's approximation of the Fresnel factor.
	vec3 fresnelSchlick(vec3 F0, float cosTheta)
	{
		return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
	}

	vec3 GetPBRColor(in vec3 worldPos, in vec3 nWorldNormal, vec3 albedoColor)
	{
		//vec3 Lo = vec3(0.0);
		vec3 V = normalize(CameraWorldPosition - worldPos);

		vec3 F0 = vec3(0.04);

		if (fs_in.tex_coords.x < 0.5)
		{
			Metallic = 0.8;	
			Roughness = 0.2;
		}
		else 
		{
			Metallic = 0.2;	
			Roughness = 0.8;
		}

		F0 = mix(F0, albedoColor, Metallic);

		vec3 N = nWorldNormal;

		/* POINT LIGHTS */

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(CameraWorldPosition - worldPos);

	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));

	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * N - Lo;

	// Direct lighting calculation for analytical lights.
	vec3 pointLighting = vec3(0);
		for (uint pointLightIndex = 0; pointLightIndex < 4; ++pointLightIndex)
		{
			// calculate per-light radiance

//			vec3 L_nonNormalized = PointLightPositionWorld[pointLightIndex] - worldPos;
//
//			vec3 L = normalize(L_nonNormalized);
//			vec3 H = normalize(V + L);
//
//			float distance = 20.0 / dot(L_nonNormalized, L_nonNormalized);
//
//			float attenuation = 1.0;
//			vec3 radiance = vec3(1) * attenuation;
//
//			 // cook-torrance brdf
//		    float NDF = DistributionGGX(nWorldNormal, H, Roughness);
//		    float G = GeometrySmith(nWorldNormal, V, L, Roughness);
//		    vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
//
//			vec3 kS = F;
//		    vec3 kD = vec3(1.0) - kS;
//		    kD *= 1.0 - Metallic;
//
//			vec3 numerator    = NDF * G * F;
//		    float denominator = 4.0 * max(dot(nWorldNormal, V), 0.0) * max(dot(nWorldNormal, L), 0.0);
//		    vec3 specular     = numerator / max(denominator, 0.001);
//
//			// add to outgoing radiance Lo
//		    float NdotL = max(dot(nWorldNormal, L), 0.0);
//		    Lo += (kD * albedoColor / PI + specular) * radiance * NdotL;


		vec3 Li = normalize(PointLightPositionWorld[pointLightIndex] - worldPos);

		// temporary
		float attenuation = 1; // for now
		vec3 lightRadiance = vec3(1); // for now 
		// temprorary

		vec3 Lradiance = lightRadiance * attenuation;

		// Half-vector between Li and Lo.
		vec3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));

		// Calculate Fresnel term for direct lighting.
		vec3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(cosLh, Roughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(cosLi, cosLo, Roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), Metallic);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		vec3 diffuseBRDF = kd * albedoColor;

		// Cook-Torrance specular microfacet BRDF.
		vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
		pointLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
		}

		return pointLighting;
	}

#endif

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
	const float offset  = 0.1;
	const float offsetStep = offset / (PCF_SAMPLES_POINT_LIGHT * 0.5);

	for (float x = -offset; x < offset; x += offsetStep)
	{
		for (float y = -offset; y < offset; y += offsetStep)
		{
			for (float z = -offset; z < offset; z += offsetStep)
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

    for (int x = -PCF_SAMPLES_DIR_LIGHT; x <= PCF_SAMPLES_DIR_LIGHT; x++)
    {
       for (int y = -PCF_SAMPLES_DIR_LIGHT; y <= PCF_SAMPLES_DIR_LIGHT; y++)
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
	#ifdef SHADING_MODEL_PBR
		vec4 totalColor = vec4(GetPBRColor(worldPos, worldNormal, albedoAndSpecular.xyz), 1.0);
	#else
		vec3 diffuseColor = GetDiffuseColor(worldPos, worldNormal);
		vec3 ambientColor = GetAmbientColor();
		vec4 totalColor = vec4(albedoAndSpecular.rgb * (diffuseColor + ambientColor), 1);
	#endif

	FragColor = totalColor;
}