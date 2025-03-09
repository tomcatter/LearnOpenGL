#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec3 vertColor;

void main() {
    FragColor = vec4(mix(ourColor, vertColor, 0.5), 1.0f);
}
