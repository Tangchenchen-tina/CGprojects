#version 330
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{   
  float len = length(FragPos.xyz - lightPos);

  len /= far_plane;
  gl_FragDepth = len;          
}  