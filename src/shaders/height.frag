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
in vec3 normal_cam;
in vec4 tangent_cam;
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
    vec3 bump = vec3(texture(map_bump, uv));
    if (bump.x == bump.y && bump.y == bump.z && bump.z == 0)
        discard;
    bump.xy = (bump.xy - 0.5) * 2;

    vec3 n_cam = normalize(normal_cam);
    vec4 t_cam = vec4(normalize(vec3(tangent_cam)), 1);
    vec3 b_cam = t_cam.w * normalize((cross(n_cam, vec3(t_cam))));

    mat3 TBN = transpose(mat3(vec3(t_cam), b_cam, n_cam));
    vec3 light_dir_tan = TBN * vec3(light.position);
    vec3 eye_dir_tan = TBN * eye_cam;
    vec3 normal_tan = normalize(bump);
    vec3 reflection_tan = TBN * reflection_cam;


    out_color = global_ambient * material.ambient;

    float n_dot_l = max(dot(normal_tan, light_dir_tan), 0);
    out_color += n_dot_l * light.param.diffuse * Kd;

    if (n_dot_l > 0.01f) {
        float e_dot_r = max(dot(normalize(eye_dir_tan),
                                normalize(reflection_tan)), 0);
        out_color += light.param.specular * material.specular *
                     pow(e_dot_r, material.shininess);
    }
}
