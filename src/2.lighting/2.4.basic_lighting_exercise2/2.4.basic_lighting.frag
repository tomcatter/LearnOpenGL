#version 330 core

// 物体颜色
uniform vec3 objectColor;

// 光照颜色
uniform vec3 lightColor;

// 观察者坐标
uniform vec3 viewPos;


in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;


out vec4 FragColor;

void main(){
    // 归一化法线
    vec3 norm = normalize(Normal);
    // 光线方向
    vec3 lightDir = normalize(LightPos - FragPos);
    // 计算光线和法线之间的点乘大小
    float diff = max(dot(norm, lightDir), 0.0f);
    // 漫反射
    vec3 diffuse = lightColor * diff;

    // 环境光
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // 高光
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}