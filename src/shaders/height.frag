//[FRAGMENT SHADER]
#version 330

in vec2 uv;

out vec4 out_color;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
  vec4 tex1 = texture(texture0, uv);
  vec4 tex2 = texture(texture1, uv);
  out_color = mix(tex1, tex2, 0.5);
}
