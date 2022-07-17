#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct PointLight {
    bool on;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // attenuation
    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS $NP$

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform vec3 shapeColor;

// function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += pointLights[i].on ? CalcPointLight(pointLights[i], norm, FragPos, viewDir) : vec3(0.0, 0.0, 0.0);

    vec3 resultFinal = result * shapeColor;
    FragColor = vec4(resultFinal, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance);

    return ambient + ((diffuse + specular)/attenuation);
}