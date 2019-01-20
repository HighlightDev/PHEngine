#version 400

#define hallo vec2(0.5, 1)
layout (location = 0) in vec3 vertex;
//layout (location = 1) in vec2 texCoord;
//layout (location = 2) in vec3 normal;

//uniform mat4 viewMatrix;
//uniform mat4 projectionMatrix;

//out vec2 texCoords;

void main()
{
	//texCoords = texCoord;
	//gl_Position = projectionMatrix * viewMatrix * vec4(vertex, 1.0);
	gl_Position = vec4(vertex, 1.0);
}