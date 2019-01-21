#version 400

layout (location = 0) in vec3 Position;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


out vec3 passTextureCoord;

void main(void)
{
    vec4 worldSpaceVertex = worldMatrix * vec4(Position, 1.0);
    passTextureCoord = Position;

	gl_Position = projectionMatrix * viewMatrix * worldSpaceVertex;
}