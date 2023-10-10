#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;

void main()
{
    TexCoord = texCoord;
    gl_Position = model * vec4(position, 1.0f);
}
