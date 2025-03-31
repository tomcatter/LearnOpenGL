#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

// 聚光灯源属性
struct Light {
    vec3 position;
    vec3 direction; // 聚光所指的方向
    float cutOff;   // 聚光半径的切光角

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

    // 光源到物体表面顶点的方向
    vec3 lightDir = normalize(light.position - FragPos);
    // 计算光源方向和片段到光源方向的夹角
    float theta = dot(lightDir, normalize(-light.direction));

    // 这里theta是cos的值，所以越小表示角度越大
    if (theta > light.cutOff) {
        // 环境光
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        // 漫反射
        vec3 norm = normalize(Normal);

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

        vec3 result = ambient + diffuse + specular + emission;
        FragColor = vec4(result, 1.0);
    } else {
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    }
}