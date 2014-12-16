//[FRAGMENT SHADER]
#version 330

struct light_source {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
};

uniform sampler2D map_pos_cam;
uniform sampler2D map_normal_cam;
uniform sampler2D map_diffuse;
uniform sampler2D map_specular;

uniform sampler2DShadow shadow_map;

uniform light_source light;
uniform float material_shininess;
uniform vec2 screen_size;

uniform mat4 cam_to_ls_bias_proj;

out vec4 out_color;

float shadow_attenuation(vec4 pos_cam)
{
    // Get position in texture coord
    vec4 position_ls = cam_to_ls_bias_proj * pos_cam;
    // Add a threshold to avoid shadow acnee
    position_ls.z -= 0.00001 * position_ls.w;
    // Using hardware PCF thanks to sampler2DShodw with linear sampling
    return textureProj(shadow_map, position_ls);
}

void main()
{
    // Get screen space texture coordinates
    vec2 text_coord = gl_FragCoord.xy / screen_size;
    // Extract data from framebuffer
    vec3 normal_cam = vec3(texture(map_normal_cam, text_coord));
    vec3 pos_cam = vec3(texture(map_pos_cam, text_coord));
    vec4 Kd = texture(map_diffuse, text_coord);
    vec4 Ks = texture(map_specular, text_coord);

    vec3 light_dir_cam = vec3(light.position);

    float n_dot_l = max(dot(normal_cam, light_dir_cam), 0);
    out_color = n_dot_l * light.diffuse * Kd;
    vec3 reflection_cam = reflect(-light_dir_cam, normal_cam);
    vec3 eye_vector_cam = -normalize(pos_cam);

    if (n_dot_l > 0) {
        float e_dot_r = max(dot(eye_vector_cam, normalize(reflection_cam)), 0);
        out_color += light.specular * Ks * pow(e_dot_r, 42);
    }
    out_color *= shadow_attenuation(vec4(pos_cam, 1));
}
