#version 400 core
out vec4 FragColor;
uniform vec4 lightColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
in mat3 TBN;

void main()
{
    FragColor = lightColor;
}