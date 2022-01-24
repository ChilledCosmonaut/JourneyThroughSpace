#version 460 core
out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec4 lightPos;
uniform vec4 lightColor;
uniform vec4 viewPos;

void main()
{
    vec3 objectColor = vec3(0.5,0.4,0.7);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor.xyz;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos.xyz - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.xyz;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos.xyz - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor.xyz;

    vec3 result = (ambient + diffuse +  specular) * objectColor;
    fragColor = vec4(result, 1.0);
}
