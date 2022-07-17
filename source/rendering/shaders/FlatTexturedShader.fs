#version 330 core

in vec3 VertexLightColor;
in vec2 FragTexCoords;

out vec4 FragColor;
// texture samplers
uniform sampler2D texture1;

void main()
{
    vec4 textureColor = texture(texture1, FragTexCoords);
    vec3 resultFinal = VertexLightColor * textureColor.rgb;
    FragColor = vec4(resultFinal, textureColor[3]);
} 