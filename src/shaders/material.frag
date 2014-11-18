//[FRAGMENT SHADER]
#version 330

struct light_param {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform light_param material;

in vec3 normal_cam;
in vec3 pos_cam;

layout (location = 0) out vec3 pos_cam_out;
layout (location = 1) out vec3 normal_cam_out;
layout (location = 2) out vec3 diffuse_out;
layout (location = 3) out vec3 specular_out;

void main()
{
    pos_cam_out = pos_cam;
    normal_cam_out = normalize(normal_cam);
    diffuse_out = vec3(material.diffuse);
    specular_out = vec3(material.specular);
}
