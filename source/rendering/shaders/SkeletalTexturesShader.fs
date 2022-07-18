#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;;

in vec2 FragTexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 color;

void main()
{    
    vec4 textured = texture(texture_diffuse1, FragTexCoords);
    vec3 result = textured.rgb * color;
    FragColor = vec4(result, 1.0f) ;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, FragColor[3]);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, FragColor[3]);
}
