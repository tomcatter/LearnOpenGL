#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main(){
    // 归一化法线
    vec3 norm = normalize(Normal);
    // 光线方向
    vec3 lightDir = normalize(lightPos - FragPos);
    // 计算光线和法线之间的点乘大小
    float diff = max(dot(norm, lightDir), 0.0f);
    // 漫反射
    vec3 diffuse = lightColor * diff;

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0f);
}