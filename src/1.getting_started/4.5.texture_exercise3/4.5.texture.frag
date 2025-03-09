#version 330 core

in vec3 vColor;
in vec2 vTexCoor;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main(){
    vec4 color1 = texture(texture1, vTexCoor);
    vec4 color2 = texture(texture2, vec2(1.0-vTexCoor.x, vTexCoor.y));
    FragColor = mix(color1, color2, 0.4);
}