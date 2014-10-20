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
    vec3 position;
};

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform light_source light;
uniform light_param material;
uniform vec4 global_ambient;

in vec2 uv;
in vec3 normal;
in vec3 reflection;

out vec4 out_color;
in vec3 light_dir;
in vec3 eye;

void main()
{
    vec3 n = normalize(normal);
    vec4 tex1 = texture(texture0, uv);
    vec4 tex2 = texture(texture1, uv);

    out_color = global_ambient * material.ambient;
    //out_color += mix(tex1, tex2, 0.5) * dot(normalize(light_dir), n);

    float n_dot_l = max(dot(n, normalize(light_dir)), 0);
    out_color += n_dot_l * light.param.diffuse * material.diffuse;

    if (n_dot_l > 0.01f) {
        float e_dot_r = max(dot(normalize(eye), normalize(reflection)), 0);
        out_color += light.param.specular * material.specular *
                     pow(e_dot_r, material.shininess);
    }
}
