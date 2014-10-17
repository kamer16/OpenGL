//[FRAGMENT SHADER]
#version 330

uniform vec3 light_dir;
uniform sampler2D texture0;
uniform sampler2D texture1;

in vec2 uv;
in vec3 normal;

out vec4 out_color;

void main()
{
  vec4 tex1 = texture(texture0, uv);
  vec4 tex2 = texture(texture1, uv);
  out_color = mix(tex1, tex2, 0.5) * dot(light_dir, normalize(normal));
  float val = max(dot(light_dir, normalize(normal)), 0);
  out_color = vec4(val, val, val, 1);
}
