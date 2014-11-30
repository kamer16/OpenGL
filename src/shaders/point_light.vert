//[VERTEX SHADER]
#version 330

uniform mat4 mvp_mat;

layout (location = 0) in vec3 in_position;

void main()
{
    gl_Position = mvp_mat * vec4(in_position, 1.0f);
}
