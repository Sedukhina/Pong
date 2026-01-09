#version 440 core

in vec2 TextureCoords;

uniform sampler2D BaseColor;

out vec4 color;

void main()
{
    color = texture(BaseColor, TextureCoords); 
}