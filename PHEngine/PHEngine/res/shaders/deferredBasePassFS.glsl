#version 400

layout (location = 0) out vec3 gBuffer_Position;
layout (location = 1) out vec3 gBuffer_Normal;
layout (location = 2) out vec4 gBuffer_AlbedoNSpecular;

in vec3 world_position;
in vec3 world_normal;
in vec2 tex_coords;

uniform sampler2D albedo;
uniform sampler2D specular;

void main()
{
	vec3 albedoColor = texture(albedo, tex_coords).rgb;
	float specularFactor = texture(specular, tex_coords).r;
	
	gBuffer_Position = world_position;
	gBuffer_Normal = normalize(world_normal);
	gBuffer_AlbedoNSpecular = vec4(albedoColor, specularFactor);
}