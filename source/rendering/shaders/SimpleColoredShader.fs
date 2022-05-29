#version 330 core
out vec4 FragColor;

in vec3 OutColor;

uniform vec3 shapeColor;

void main()
{
	FragColor = vec4(OutColor * shapeColor, 1.0f);
}