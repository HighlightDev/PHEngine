#version 400

#define MaxWeights 3
#define MaxBones 55
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

layout(location = 6) in vec3 blendWeights;
layout(location = 7) in ivec3 blendIndices;

out vec3 world_position;
out vec3 world_normal;
out vec2 tex_coords;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 bonesMatrices[MaxBones];

uniform bool isSkeletal;

void main(void)
{
	vec4 worldPosition = vec4(position, 1.0);

	if (isSkeletal)
	{
		vec4 localSpaceSkinnedVertex = vec4(0);
		 for (int i = 0; i < MaxWeights; i++)
		{
			int blendIndex = blendIndices[i];
			if (blendIndex < 0)
				continue;

			float blendWeight = blendWeights[i];
			localSpaceSkinnedVertex += ((bonesMatrices[blendIndex]  * worldPosition) * blendWeight);
		}
		worldPosition = worldMatrix * localSpaceSkinnedVertex;
	}
	else
	{
		worldPosition = worldMatrix * worldPosition;
	}

	world_position = worldPosition.xyz;
	world_normal = (worldMatrix * vec4(normal, 0.0)).xyz;
	tex_coords = texCoord;

	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}