#version 330

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

uniform mat4 gWVP;
//uniform vec4 rightNormal;
//uniform vec4 upNormal;

//out vec4 outColor;
out vec2 outTex;

void main()
{
  gl_Position = gWVP * vec4(vertexPosition_modelspace,1);
  outTex = vertexUV;
}