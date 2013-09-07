#version 330 core

uniform vec3 model_color;
out vec3 color;

void main()
{
  color = model_color;
}
