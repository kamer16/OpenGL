//[VERTEX SHADER]
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

uniform mat4 projMat;
uniform mat4 model_view_mat;
uniform mat3 normal_mat;
uniform light_source light;


layout (location = 0) in vec2 in_uv;
layout (location = 1) in vec3 in_position;
layout (location = 2) in vec3 in_norm;

out vec2 uv;
out vec3 normal;
// vector in between light_source and eye position
out vec3 half_vector;

void main()
{
    gl_Position = projMat * model_view_mat * vec4(in_position, 1.0f);
    // Vector pointing from object to middle of eye and light
    // (eye_pos - vertex) + (light.position i-e light.direction)
    half_vector = normalize(light.position - vec3(normalize(gl_Position)));
    uv = in_uv;
    normal = normalize(normal_mat * in_norm);
}
