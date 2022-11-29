#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices_0;
uniform mat4 shadowMatrices_1;
uniform mat4 shadowMatrices_2;
uniform mat4 shadowMatrices_3;
uniform mat4 shadowMatrices_4;
uniform mat4 shadowMatrices_5;

out vec4 FragPos;

void main(){
  for(int face = 0; face<6; ++face){
    gl_Layer = face;
    for(int i=0; i<3; ++i){
      FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMatrices_0 * FragPos;
      if (face == 1){
        gl_Position = shadowMatrices_1 * FragPos;
      }else if (face == 2){
        gl_Position = shadowMatrices_2 * FragPos;
      }else if (face == 3){
        gl_Position = shadowMatrices_3 * FragPos;
      }else if (face == 4){
        gl_Position = shadowMatrices_4 * FragPos;
      }else if (face == 5){
        gl_Position = shadowMatrices_5 * FragPos;
      }
      EmitVertex();
    }
    EndPrimitive();
  }
}