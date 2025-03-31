#version 330 core

#define NR_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

// 平行光属性
struct DirLight {
    vec3 direction;  // 方向
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 点光源属性
struct PointLight {
    vec3 poistion;
    float constant; // 计算衰减的常量
    float linear;  // 一次项
    float quadratic; // 二次项系数

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 聚光灯光源属性
struct SpotLight {
    vec3 position;
    vec3 direction; // 聚光所指的方向
    float cutOff;   // 聚光半径的切光角
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;

uniform float matrixmove;
uniform float matrixlight;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;


// 计算平行光照射颜色
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0f);
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return ambient + diffuse + specular;
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.poistion - fragPos);
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0f);
    // 高光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    //计算衰减
    float distance = length(light.poistion - fragPos);
    float attenuation = 1 /(light.constant + light.linear* distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}


vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0f);
    // 高光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // 计算衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutoff;
    float intensity = clamp((theta-light.outerCutoff)/epsilon, 0.0f, 1.0f);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    result += calcSpotLight(spotLight, norm, FragPos, viewDir);

    // emission
    float x = min(1 - step(TexCoords.x, 0.1), step(TexCoords.x, 0.9));
    float y = min(1 - step(TexCoords.y, 0.1), step(TexCoords.y, 0.9));
    vec3 emission = min(x, y) * matrixlight * texture(material.emission, vec2(TexCoords.x, TexCoords.y + matrixmove)).rgb;

    result += emission;
    FragColor = vec4(result, 1.0);
}