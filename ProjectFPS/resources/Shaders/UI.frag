#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec4 Color;
    vec3 TexCoord0;
} fsIn;
  
uniform vec4 color;

void main()
{
    FragColor = color;
}