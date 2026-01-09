#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

uniform mat4 CameraMatrix;
uniform mat4 ModelMatrix;

out vec2 TextureCoords;

void main()
{
   gl_Position =  CameraMatrix * ModelMatrix * vec4(position, 1.0f);
   TextureCoords = textureCoords;
};