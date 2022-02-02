#version 460 core
out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    fragColor = vec4(1.0); // set all 4 vector values to 1.0
}
