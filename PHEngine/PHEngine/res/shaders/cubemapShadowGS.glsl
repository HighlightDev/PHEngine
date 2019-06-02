#version 400

#define CubemapSides 6
layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;


uniform mat4 shadowViewMatrices[CubemapFaces];
uniform mat4 shadowProjectionMatrices[CubemapFaces];

out vec3 FragPos;

void main(void)
{
	for (int face = 0; i < CubemapFaces; ++face)
	{
		for (int i = 0; i < 3; ++i)
		{
			FragPos = gl_in[i].gl_Position.xyz;
			gl_Position =  shadowProjectionMatrices[face] * shadowViewMatrices[face] * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}