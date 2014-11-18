//[FRAGMENT SHADER]
#version 330

uniform sampler2D map_Kd;
uniform sampler2D map_Ks;
uniform sampler2D map_bump;
uniform sampler2D map_d;

in vec2 text_coord;
in vec3 normal_cam;
in vec4 tangent_cam;
in vec3 pos_cam;

layout (location = 0) out vec3 pos_cam_out;
layout (location = 1) out vec3 normal_cam_out;
layout (location = 2) out vec3 diffuse_out;
layout (location = 3) out vec3 specular_out;

void main()
{
    vec4 d = texture(map_d, text_coord);
    if (d.x + d.y + d.z == 0)
        discard;
    vec3 bump = vec3(texture(map_bump, text_coord));
    bump.xy = (bump.xy - 0.5) * 2;

    vec3 n_cam = normalize(normal_cam);
    vec4 t_cam = vec4(normalize(vec3(tangent_cam)), 1);
    vec3 b_cam = t_cam.w * normalize((cross(n_cam, vec3(t_cam))));

    mat3 TBN = mat3(vec3(t_cam), b_cam, n_cam);

    normal_cam_out = normalize(TBN * bump);
    pos_cam_out = pos_cam;
    diffuse_out = vec3(texture(map_Kd, text_coord));
    specular_out = vec3(texture(map_Ks, text_coord));
}
