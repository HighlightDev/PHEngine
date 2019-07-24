#version 400

#include "materialCommon.incl"

out VS_OUT
{
	vec3 world_normal;
	vec3 world_tangent;
	vec3 world_bitangent;
	vec3 world_position;
	vec2 tex_coords;
} vertex_shader_result;

out MATERIAL_VS_OUTPUT MatOutput;

void main(void)
{
	MatOutput = VertexFactoryGetMaterialOutput();

	vertex_shader_result.world_normal = GetLocalToWorldSpaceNormal();
	vertex_shader_result.world_tangent = GetLocalToWorldSpaceTangent();
	vertex_shader_result.world_bitangent = GetLocalToWorldSpaceBitangent();
	vertex_shader_result.tex_coords = MatOutput.TextureCoordinates.xy;

	vertex_shader_result.world_position = MatOutput.WorldCoordinates;

	vec4 clippedCoordinates = MatOutput.ClippedCoordinates;

	gl_Position = clippedCoordinates;
}