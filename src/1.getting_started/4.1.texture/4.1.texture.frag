#version 330 core

in vec3 vColor;
in vec2 vTexCoor;

uniform sampler2D texture1;

out vec4 FragColor;

void main(){
    vec4 color = texture(texture1, vTexCoor);
    FragColor = mix(color, vec4(vColor, 1.0), 0.6);
}