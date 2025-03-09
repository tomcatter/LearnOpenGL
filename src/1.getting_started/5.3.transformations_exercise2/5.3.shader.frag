#version 330 core

uniform sampler2D texture1;

in vec2 vTexCoord;

out vec4 FragColor;

void main() {
    FragColor = texture(texture1, vTexCoord);
}