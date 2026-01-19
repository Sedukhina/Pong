#version 330 core

in vec2 TexCoord;

uniform sampler2D FontAtlas;
uniform vec4 Color;

out vec4 fragColor;

void main()
{
    fragColor = vec4(texture(FontAtlas, TexCoord).r) * Color;
}