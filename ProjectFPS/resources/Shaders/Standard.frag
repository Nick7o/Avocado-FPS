#version 330 core
#include include/engine.glsl

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec4 Color;
    vec3 Normal;
    vec3 Tangent;
    vec3 TexCoord0;
    mat3 TBN;
} fsIn;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D maskMap;
  
void main()
{
    vec4 diffuseColor = texture(diffuseMap, fsIn.TexCoord0.xy);
    vec3 normal = texture(normalMap, fsIn.TexCoord0.xy).rgb * 2.0 - 1.0;  
    normal = normalize(fsIn.TBN * normal); 

    vec3 mask = texture(maskMap, fsIn.TexCoord0.xy).rgb;
    float ao = mask.r;
    float roughness = mask.g;
    float metallic = mask.b;

    vec3 viewDir = normalize(viewPos - fsIn.FragPos);
    Lighting worldLighting = CalculateLighting(normal, viewDir, fsIn.FragPos, 64 - (roughness * 64), roughness);

    FragColor = diffuseColor * vec4(EvaluateLighting(worldLighting) * pow(ao, 1.0), 1.0);
    //FragColor = vec4(vec3(mask.g), 1.0);// dirLighting * diffuseColor;
    //FragColor = vec4(texture(normalMap, fsIn.TexCoord0.xy * 16).rgb, 1);
    //FragColor = vec4(fsIn.Normal, 1);
    //FragColor = vec4(fsIn.Tangent + 0.5 * 0.5, 1);
    //FragColor = vec4(texture(directionalLightShadowMap, fsIn.TexCoord0.xy).rrr, 1.0);
}