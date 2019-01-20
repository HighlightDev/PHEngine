#version 400

layout (location = 0) out vec4 FragColor;

//uniform sampler2D albedo;

//in vec2 texCoords;

void main()
{
	//FragColor = texture(albedo, texCoords);
	FragColor = vec4(1, 0, 1, 1);
}