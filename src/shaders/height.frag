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
uniform sampler2D map_bump;

uniform light_source light;
uniform light_param material;
uniform vec4 global_ambient;

in vec2 uv;
in vec3 normal;
in vec3 reflection;

out vec4 out_color;
in vec3 eye;

void main()
{
    vec3 n = normalize(normal);
    vec4 Ka = texture(map_Ka, uv);
    vec4 Kd = texture(map_Kd, uv);
    vec4 Ks = texture(map_Ks, uv);
    vec4 d = texture(map_d, uv);
    vec4 bump = texture(map_bump, uv);

    out_color = global_ambient * material.ambient;
    //out_color += mix(Ka, Kd, 0.5) * dot(normalize(light.position), n);

    float n_dot_l = max(dot(n, vec3(light.position)), 0);
    out_color += n_dot_l * light.param.diffuse * material.diffuse;

    if (n_dot_l > 0.01f) {
        float e_dot_r = max(dot(normalize(eye), normalize(reflection)), 0);
        out_color += light.param.specular * material.specular *
                     pow(e_dot_r, material.shininess);
    }
    out_color = mix(Kd, out_color, 0.8);
}
