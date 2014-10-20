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
    vec3 position;
};

uniform mat4 projMat;
uniform mat4 model_view_mat;
uniform mat4 view_mat;;
uniform mat3 normal_mat;
uniform light_source light;

layout (location = 0) in vec2 in_uv;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec3 in_norm;

out vec2 uv;
out vec3 normal;
// reflection vector caused by light
out vec3 reflection;
out vec3 light_dir;
// direction of vertex to eye
out vec3 eye;

void main()
{
    uv = in_uv;
    normal = normalize(normal_mat * in_norm);
    // TODO should be computed in user code not here
    light_dir = normalize(normal_mat * light.position);
    // Convert vertex to eye space ==> eye_position == (0, 0, 0)
    eye = normalize(-vec3(view_mat * vec4(in_position, 1)));

    reflection = reflect(-light_dir,  normal);
    gl_Position = projMat * model_view_mat * vec4(in_position, 1.0f);
}
