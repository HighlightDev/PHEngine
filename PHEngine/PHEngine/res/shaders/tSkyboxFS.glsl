#version 400

layout (location = 0) out vec4 FragColor;

uniform samplerCube dayTexture;
uniform samplerCube nightTexture;

in vec3 passTextureCoord;

void main(void)
{
	vec4 texDayColour = texture(dayTexture, passTextureCoord);

	FragColor = texDayColour;
}
