#version 330 core
out vec4 FragColor;

in vec2 outTexCoord;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, outTexCoord);
};