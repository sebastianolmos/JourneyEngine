#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 VertexLightColor;
in vec2 FragTexCoords;

// texture samplers
uniform sampler2D texture1;

void main()
{
    vec4 textureColor = texture(texture1, FragTexCoords);
    vec3 resultFinal = VertexLightColor * textureColor.rgb;

    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(resultFinal, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(resultFinal, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    FragColor = vec4(resultFinal, textureColor[3]);
} 