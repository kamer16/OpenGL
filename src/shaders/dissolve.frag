//[FRAGMENT SHADER]
#version 330

in vec2 text_coord;
in vec3 normal_cam;
in vec3 pos_cam;

layout (location = 0) out vec3 pos_cam_out;
layout (location = 1) out vec3 normal_cam_out;
layout (location = 2) out vec3 diffuse_out;
layout (location = 3) out vec3 specular_out;

uniform sampler2D map_Kd;
uniform sampler2D map_Ks;
uniform sampler2D map_d;

void main()
{
    vec4 d = texture(map_d, text_coord);
    if (d.x + d.y + d.z == 0)
        discard;
    pos_cam_out = pos_cam;
    normal_cam_out = normalize(normal_cam);
    diffuse_out = vec3(texture(map_Kd, text_coord));
    specular_out = vec3(texture(map_Kd, text_coord));
}
