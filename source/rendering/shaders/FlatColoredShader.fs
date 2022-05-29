#version 330 core

flat in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
} 