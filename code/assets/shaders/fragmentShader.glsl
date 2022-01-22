#version 460 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixingFactor;

void main()
{
    fragColor = mix(texture(texture1, TexCoord), texture(texture2, 1 - TexCoord), 0.5f);
}
