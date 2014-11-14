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
layout (location = 3) in vec4 in_tan;

out vec2 uv;
out vec3 normal_cam;
out vec4 tangent_cam;
// reflection vector caused by light
out vec3 reflection_cam;
// direction of vertex to eye
out vec3 eye_cam;
out vec3 light_dir_cam;

void main()
{
    light_dir_cam = vec3(light.position);
    uv = in_uv;
    // normal in eye space
    normal_cam = normalize(normal_mat * in_norm);
    // tangent in eye space
    tangent_cam = vec4(normalize(normal_mat * vec3(in_tan)), in_tan.w);
    // Convert vertex to eye space ==> eye_position == (0, 0, 0)
    eye_cam = normalize(-vec3(mv_mat * vec4(in_position, 1)));

    // light in eye space
    reflection_cam = reflect(-vec3(light_dir_cam),  normal_cam);
    gl_Position = mvp_mat * vec4(in_position, 1.0f);
}
