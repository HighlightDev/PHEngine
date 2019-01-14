#version 400

#define testable_version 11.0
#define hallo vec2(0.5, 1)

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

void main()
{
	highp float;
	gl_Position = vec4(vertex, 1.0);
}