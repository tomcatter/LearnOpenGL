#version 330 core

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


uniform Material material;
uniform Light light;
uniform vec3 viewPos;


in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * material.diffuse * diff;

    // 高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specu = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 spcular = specu * light.specular * material.specular;

    vec3 result = ambient + diffuse + spcular;
    FragColor = vec4(result, 1.0);
}