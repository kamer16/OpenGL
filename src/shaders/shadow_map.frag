//[FRAGMENT SHADER]
#version 330

struct light_param {
    vec4 ambient;
};

uniform light_param material;

out vec4 out_color;

float LinearizeDepth(float z)
{
    float n = 1;
    float f = 500.0f;
    return z;
    return (2.0f * n) / (f + n - z * (f - n));
}

void main()
{
  out_color = vec4(LinearizeDepth(gl_FragCoord.z));
}
