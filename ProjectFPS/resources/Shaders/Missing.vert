#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aUV0;
  
out vec3 _FragPos;
out vec4 _Color;
out vec3 _Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    _FragPos = vec3(view * model * vec4(aPos, 1.0));
    _Color = vec4(aColor, 1.0);
    _Normal = mat3(transpose(inverse(view * model))) * aNormal;
}