#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec3 LightingColor;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    //
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    // 法线
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 漫返射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffus = diff * lightColor;

    // 镜面反射
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 spcular = specularStrength * spec * lightColor;
    LightingColor = ambient + diffus + spcular;
}