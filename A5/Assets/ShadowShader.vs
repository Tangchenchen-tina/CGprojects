#version 330
in vec3 position;
in vec3 normal;

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 lightProjection;
uniform mat4 model;

void main()
{
    gl_Position = lightProjection * model * vec4(position, 1.0);
}