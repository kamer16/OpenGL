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

uniform sampler2D map_Ka;
uniform sampler2D map_Kd;
uniform sampler2D map_Ks;
uniform sampler2D map_d;

uniform light_source light;
uniform light_param material;
uniform vec4 global_ambient;

in vec2 uv;
in vec3 normal_cam;
in vec3 reflection_cam;
in vec3 light_dir_cam;

out vec4 out_color;
in vec3 eye_cam;

void main()
{
    vec4 Ka = texture(map_Ka, uv);
    vec4 Kd = texture(map_Kd, uv);
    vec4 Ks = texture(map_Ks, uv);
    vec4 d = texture(map_d, uv);
    if (d.x + d.y + d.z == 0)
        discard;

    vec3 n_cam = normalize(normal_cam);
    vec3 light_dir_cam = vec3(light.position);

    out_color = global_ambient * Ka;

    float n_dot_l = max(dot(n_cam, light_dir_cam), 0);
    out_color += n_dot_l * light.param.diffuse * Kd;

    if (n_dot_l > 0.01f) {
        float e_dot_r = max(dot(normalize(eye_cam),
                                normalize(reflection_cam)), 0);
        out_color += light.param.specular * Ks *
                     pow(e_dot_r, material.shininess);
    }
}
