#version 330 core

out vec3 color;
in vec2 UV;
 
uniform sampler2D renderedTexture;
uniform float time;
 
void main(){
    color = texture2D( renderedTexture, UV).xyz;
}