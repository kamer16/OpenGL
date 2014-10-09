//[VERTEX SHADER]
#version 330

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

layout (location = 0) in vec2 in_uv; // matches gl::EnableVertexAttribArray(1);
layout (location = 1) in vec3 in_position; // matches gl::EnableVertexAttribArray(0);

out vec2 uv;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(in_position, 1.0);
    uv = in_uv;
}
