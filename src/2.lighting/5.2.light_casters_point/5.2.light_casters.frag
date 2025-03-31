#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

// 点光源属性
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;  // 常数项
    float linear;    // 一次项系数
    float quadratic; // 二次项系数
};


uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform float matrixmove;
uniform float matrixlight;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main() {

    // 计算点光源的衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, TexCoords)) * diff;

    // 高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specu = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specu * light.specular * vec3(texture(material.specular, TexCoords));

    // emission
    float x = min(1 - step(TexCoords.x, 0.1), step(TexCoords.x, 0.9));
    float y = min(1 - step(TexCoords.y, 0.1), step(TexCoords.y, 0.9));
    vec3 emission = min(x, y) * matrixlight * texture(material.emission, vec2(TexCoords.x, TexCoords.y + matrixmove)).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}