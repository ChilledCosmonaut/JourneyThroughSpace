#version 460 core
out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec4 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform Material material;
uniform Light light;

uniform vec4 viewPos;

void main()
{
    vec4 ambient = light.ambient * texture(material.diffuse, TexCoords);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position.xyz - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords));

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos.xyz - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec4 specular = light.specular * (spec * texture(material.specular, TexCoords));

    vec4 emission = texture(material.emission, TexCoords);

    vec4 result = ambient + diffuse +  specular + emission;
    fragColor = result;
}
