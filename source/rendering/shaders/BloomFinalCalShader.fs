#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform bool hdr;
uniform float exposure;
uniform float gamma;

void main()
{             
    vec3 hdrColor = texture(scene, TexCoords).rgb;    
    float alpha = texture(scene, TexCoords)[3];
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if (hdr) {
        if(bloom)
            hdrColor += bloomColor; // additive blending
        // tone mapping
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, alpha);
    }
    else {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, alpha);
    }
    
}