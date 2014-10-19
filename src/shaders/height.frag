//[FRAGMENT SHADER]
#version 330

struct light_param {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 shininess;
};
struct light_source {
    light_param param;
    vec3 position;
    // vector in between light_source and eye position
    vec4 half_vector;
};

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform light_source light;
uniform light_param material;
uniform vec4 global_ambient;

in vec2 uv;
in vec3 normal;

out vec4 out_color;

void main()
{
    vec4 tex1 = texture(texture0, uv);
    vec4 tex2 = texture(texture1, uv);
    out_color = mix(tex1, tex2, 0.5) * dot(light.position, normalize(normal));
    float val = max(dot(light.position, normalize(normal)), 0);
    out_color = vec4(val, val, val, 1) + vec4(uv, 0, 0) * 0.001f +
        global_ambient;
}
