#version 330 core

in vec3 vColor;
in vec2 vTexCoor;

uniform float mixValue;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main(){
    vec4 color1 = texture(texture1, vTexCoor);
    vec4 color2 = texture(texture2, vTexCoor);
    FragColor = mix(color1, color2, abs(mixValue));
}