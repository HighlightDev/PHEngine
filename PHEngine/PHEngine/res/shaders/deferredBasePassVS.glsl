#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 world_position;
out vec3 world_normal;
out vec2 tex_coords;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	vec4 worldPosition = worldMatrix * vec4(position, 1.0);
	world_position = worldPosition.xyz;
	world_normal = (worldMatrix * vec4(normal, 0.0)).xyz;
	tex_coords = texCoord;
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
