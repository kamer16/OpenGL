//[VERTEX SHADER]
#version 330

uniform mat4 projMat;
uniform mat4 model_view_mat;

uniform mat3 normal_mat;

layout (location = 0) in vec2 in_uv;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec3 in_norm;

out vec2 uv;
out vec3 normal;

void main()
{
    gl_Position = projMat * model_view_mat * vec4(in_position, 1.0);
    uv = in_uv;
    normal = normalize(normal_mat * in_norm);
}
