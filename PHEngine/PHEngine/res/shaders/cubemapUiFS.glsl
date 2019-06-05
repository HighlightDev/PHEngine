#version 400

layout (location = 0) out vec4 FragColor;

in vec2 texCoords;

uniform samplerCube cubemapSampler;
uniform sampler2D worldPositionGBuffer;

uniform mat4 screenSpaceMatrix;
uniform bool bPerspectiveDepthTexture;
uniform float projectionFarPlane;
uniform vec3 pointLightPosition;

float FetchDepthFromCubemap(in vec3 fragPos)
{
	vec3 fragToLight = pointLightPosition - fragPos;
	
	// assume that depth in cubemap is linear in range [0,1]
	float shadowmapDepth = texture(cubemapSampler, fragToLight).r;
	return shadowmapDepth;
}

void main(void)
{
   vec4 fragPos = texture(worldPositionGBuffer, texCoords);
	   
   vec4 color = vec4(0);

   if (bPerspectiveDepthTexture)
   {
		color = vec4(FetchDepthFromCubemap(fragPos.xyz));
   }
   else
   {
		color = texture(cubemapSampler, fragPos.xyz);
   }

   FragColor = color;
}