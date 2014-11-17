//[FRAGMENT SHADER]
#version 330

in vec3 text_coord;
in vec3 normal_cam;
in vec3 pos_cam;

layout (location = 0) out vec3 pos_cam_out;
layout (location = 1) out vec3 normal_cam_out;
layout (location = 2) out vec3 text_coord_out;

void main()
{
    pos_cam_out = pos_cam;
    normal_cam_out = normalize(normal_cam);
    text_coord_out = text_coord;
}
