//[FRAGMENT SHADER]
#version 330

struct light_param {
    vec4 ambient;
};

uniform light_param material;

out vec4 out_color;

void main()
{
  out_color =  material.ambient;
}
