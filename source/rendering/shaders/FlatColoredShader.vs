#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec4 VertexColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 shapeColor;

void main()
{
    vec3 vertexPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(vertexPos, 1.0);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse 
    vec3 norm = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec3 lightDir = normalize(light.position - vertexPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - vertexPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    vec3 result = (ambient + diffuse + specular) * shapeColor;
    VertexColor = vec4(result, 1.0);    
}