#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D diff;

void main()
{
    FragColor = texture(diff, TexCoord);
}
