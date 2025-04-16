#version 330 core
out vec4 FragColor;

in vec2 vTexCoords;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, vTexCoords);
    if (texColor.w < 0.1)
        discard;
    FragColor = texColor;
}