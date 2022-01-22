#version 460 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 lightColor;

uniform float mixingFactor;

void main()
{
    fragColor = lightColor * texture(texture1, TexCoord);
}
