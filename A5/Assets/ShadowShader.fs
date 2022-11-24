#version 330

out vec4 output_color;

void main()
{
 //fragmentdepth = gl_FragCoord.z;
 //gl_FragDepth = gl_FragCoord.z;
  output_color = vec4(gl_FragCoord.z);
}