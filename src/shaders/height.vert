//[VERTEX SHADER]
#version 330

struct light_param {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
struct light_source {
    light_param param;
    vec4 position;
};

uniform mat4 mvp_mat;
uniform mat4 mv_mat;
uniform mat3 normal_mat;

uniform light_source light;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec2 in_uv;

out vec2 uv;
out vec3 normal;
// reflection vector caused by light
out vec3 reflection;
// direction of vertex to eye
out vec3 eye;

void main()
{
    uv = in_uv;
    normal = normalize(normal_mat * in_norm);
    // Convert vertex to eye space ==> eye_position == (0, 0, 0)
    eye = normalize(-vec3(mv_mat * vec4(in_position, 1)));

    reflection = reflect(-vec3(light.position),  normal);
    gl_Position = mvp_mat * vec4(in_position, 1.0f);
}
