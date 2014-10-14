//[VERTEX SHADER]
#version 330

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

layout (location = 0) in vec2 in_uv;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec4 in_color;

out vec2 uv;
out vec4 color;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(in_position, 1.0);
    uv = in_uv;
    color = vec4(0, 1, 0, 1);
}
