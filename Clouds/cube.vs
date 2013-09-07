#version 330 core

in vec3 vertexPosition_modelspace;
uniform mat4 MVP;
 
void main(){
  vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
  gl_Position = MVP * v;
}