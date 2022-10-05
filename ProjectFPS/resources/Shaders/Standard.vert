#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec4 aColor0;
in vec3 aUV0;
in vec3 aTangent;
  
out VS_OUT {
    vec3 FragPos;
    vec4 Color;
    vec3 Normal;
    vec3 Tangent;
    vec3 TexCoord0;
    mat3 TBN;
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

    vsOut.Normal = normalize(vec3(model * vec4(aNormal, 0.0)));
    vsOut.Tangent = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(cross(aNormal, aTangent), 0.0)));

    vsOut.TBN = mat3(vsOut.Tangent, B, vsOut.Normal);
}