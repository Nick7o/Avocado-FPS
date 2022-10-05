#version 330 core
in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    TexCoords = aPos;
} 