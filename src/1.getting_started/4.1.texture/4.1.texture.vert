#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoor;

out vec3 vColor;
out vec2 vTexCoor;

uniform float offset;

void main() {
    gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
    vColor = aColor;
    vTexCoor = aTexCoor;
}