#version 460 core

out vec4 Color;

in vec4 fragColor;

void main()
{
    Color = fragColor;
}
