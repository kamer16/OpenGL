//[VERTEX SHADER]
#version 330

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

out vec4 color;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(in_position, 1.0);
    color = vec4(in_color, 1);
}
