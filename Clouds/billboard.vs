#version 330 core

in vec3 vertexPosition_modelspace;
uniform mat4 MVP;
out vec2 UV;
 
void main(){
  vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
  gl_Position = MVP * v;
  UV = (vertexPosition_modelspace.xy+vec2(1,1))/2.0;
}