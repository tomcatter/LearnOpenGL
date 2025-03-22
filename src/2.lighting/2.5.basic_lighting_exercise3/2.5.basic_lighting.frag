#version 330 core

// 物体颜色
uniform vec3 objectColor;


in vec3 LightingColor;

out vec4 FragColor;

void main(){
    FragColor = vec4(LightingColor*objectColor, 1.0f);
}