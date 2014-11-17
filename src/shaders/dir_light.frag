//[FRAGMENT SHADER]
#version 330

struct light_source {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
};

uniform sampler2D map_Kd;
uniform sampler2D map_Ks;

uniform sampler2D map_pos_cam;
uniform sampler2D map_normal_cam;
uniform sampler2D map_text_coord;

uniform light_source light;
uniform float material_shininess;
uniform vec2 screen_size;

in vec2 uv;
in vec3 normal_cam;
in vec3 reflection_cam;
in vec3 light_dir_cam;

out vec4 out_color;
in vec3 eye_cam;

void main()
{
    // Get screen space texture coordinates
    vec2 text_coord = gl_FragCoord.xy / screen_size;
    // Extract data from framebuffer
    vec3 normal_cam = vec3(texture(map_normal_cam, text_coord));
    vec3 pos_cam = vec3(texture(map_pos_cam, text_coord));
    vec2 uv = vec2(texture(map_text_coord, text_coord));
    // Compute diffuse and specular textures
    vec4 Kd = texture(map_Kd, uv);
    vec4 Ks = texture(map_Ks, uv);

    vec3 light_dir_cam = vec3(light.position);

    float n_dot_l = max(dot(normal_cam, light_dir_cam), 0);
    out_color += n_dot_l * light.diffuse * Kd;
    vec3 reflection_cam = reflect(-light_dir_cam, normal_cam);
    vec3 eye_vector_cam = -normalize(pos_cam);

    if (n_dot_l > 0) {
        float e_dot_r = max(dot(eye_vector_cam, normalize(reflection_cam)), 0);
        out_color += light.specular * Ks * pow(e_dot_r, material_shininess);
    }
}