//[VERTEX SHADER]
#version 330

uniform mat4 mvp_mat;
uniform mat4 mv_mat;
uniform mat3 normal_mat;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec4 in_tan;

out vec2 text_coord;
out vec3 normal_cam;
out vec3 pos_cam;
out vec4 tangent_cam;

void main()
{
    text_coord = in_uv;
    // normal in eye space
    normal_cam = normalize(normal_mat * in_norm);
    // tangent in eye space
    tangent_cam = vec4(normalize(normal_mat * vec3(in_tan)), in_tan.w);
    pos_cam = vec3(mv_mat * vec4(in_position, 1.0f));
    gl_Position = mvp_mat * vec4(in_position, 1.0f);
}
