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
in vec3 half_vector;

out vec4 out_color;

void main()
{
    vec3 half_v = normalize(half_vector);
    vec3 n = normalize(normal);
    vec4 tex1 = texture(texture0, uv);
    vec4 tex2 = texture(texture1, uv);
    out_color = mix(tex1, tex2, 0.5) * dot(light.position, n);

    float n_dot_l = max(dot(n, light.position), 0);
    out_color = n_dot_l * light.param.diffuse * material.diffuse;

    float n_dot_hv = max(dot(n, half_v), 0);
    out_color += light.param.specular * material.specular *
                pow(n_dot_hv, material.shininess);
    out_color += global_ambient * material.ambient;
}
