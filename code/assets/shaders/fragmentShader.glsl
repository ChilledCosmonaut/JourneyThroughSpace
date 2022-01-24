#version 460 core
out vec4 fragColor;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 lightPos;
uniform vec4 lightColor;

uniform float mixingFactor;

void main()
{
    vec3 objectColor = vec3(0.5,0.4,0.7);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor.xyz;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos.xyz - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.xyz;

    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}
