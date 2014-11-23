//[FRAGMENT SHADER]
#version 330

struct light_source {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    float const_att;
    float lin_att;
    float quad_att;
};

uniform sampler2D map_pos_cam;
uniform sampler2D map_normal_cam;
uniform sampler2D map_diffuse;
uniform sampler2D map_specular;

uniform light_source light;
uniform float material_shininess;
uniform vec2 screen_size;

out vec4 out_color;

void main()
{
    // Get screen space texture coordinates
    vec2 text_coord = gl_FragCoord.xy / screen_size;
    // Extract data from framebuffer
    vec3 normal_cam = vec3(texture(map_normal_cam, text_coord));
    vec3 pos_cam = vec3(texture(map_pos_cam, text_coord));
    vec4 Kd = texture(map_diffuse, text_coord);
    vec4 Ks = texture(map_specular, text_coord);

    vec3 light_pos_cam = vec3(light.position);
    vec3 light_dir_cam = normalize(light_pos_cam - pos_cam);

    float n_dot_l = max(dot(normal_cam, light_dir_cam), 0);
    float dist = length(pos_cam - light_pos_cam);
    float attenuation = 1 / (light.const_att + light.lin_att * dist +
                             light.quad_att * dist * dist);

    out_color = n_dot_l * light.diffuse * Kd * attenuation;
    vec3 reflection_cam = reflect(-light_dir_cam, normal_cam);
    vec3 eye_vector_cam = -normalize(pos_cam);

    if (n_dot_l > 0) {
        float e_dot_r = max(dot(eye_vector_cam, normalize(reflection_cam)), 0);
        out_color += light.specular * Ks * pow(e_dot_r, 60) * attenuation;
    }
}
