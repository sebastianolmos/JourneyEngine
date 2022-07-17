#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 VertexColor;

void main()
{
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(VertexColor, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(VertexColor, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    FragColor = vec4(VertexColor, 1.0f);
} 