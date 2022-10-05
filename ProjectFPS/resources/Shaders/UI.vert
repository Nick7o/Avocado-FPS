#version 330 core

in vec3 aPos;
in vec4 aColor0;
in vec3 aUV0;
  
out VS_OUT {
    vec3 FragPos;
    vec4 Color;
    vec3 TexCoord0;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 uv0Tilling;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vsOut.FragPos = vec3(model * vec4(aPos, 1.0));
    vsOut.TexCoord0 = aUV0 * uv0Tilling;
    vsOut.Color = aColor0;
}