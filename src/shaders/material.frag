//[FRAGMENT SHADER]
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

uniform light_source light;
uniform light_param material;
uniform vec4 global_ambient;

in vec3 normal_cam;
in vec3 reflection_cam;
in vec3 light_dir_cam;

out vec4 out_color;
in vec3 eye_cam;

void main()
{
    vec3 n_cam = normalize(normal_cam);
    vec3 light_dir_cam = vec3(light.position);

    out_color = global_ambient * material.ambient;

    float n_dot_l = max(dot(n_cam, light_dir_cam), 0);
    out_color += n_dot_l * light.param.diffuse * material.diffuse;

    if (n_dot_l > 0.01f) {
        float e_dot_r = max(dot(normalize(eye_cam),
                                normalize(reflection_cam)), 0);
        out_color += light.param.specular * material.specular *
                     pow(e_dot_r, material.shininess);
    }
}
