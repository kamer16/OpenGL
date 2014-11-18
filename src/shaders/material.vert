//[VERTEX SHADER]
#version 330

uniform mat4 mvp_mat;
uniform mat4 mv_mat;
uniform mat3 normal_mat;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_norm;

out vec3 normal_cam;
out vec3 pos_cam;

void main()
{
    normal_cam = normalize(normal_mat * in_norm);
    pos_cam = vec3(mv_mat * vec4(in_position, 1.0f));
    gl_Position = mvp_mat * vec4(in_position, 1.0f);
}
