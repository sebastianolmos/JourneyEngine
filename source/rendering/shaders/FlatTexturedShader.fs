#version 330 core

in vec3 VertexLightColor;
in vec2 FragTexCoords;

out vec4 FragColor;
// texture samplers
uniform sampler2D texture1;

void main()
{
    vec4 textureColor = texture(texture1, FragTexCoords);
    FragColor = vec4(VertexLightColor, 1.0) * textureColor;
} 