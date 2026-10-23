#version 400 core
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
uniform vec3 uniformColor;
struct Light{
    vec4 color;
    vec3 position;
    float ambientIntensity;
};
uniform Light lights[20];

uniform int lightsAmount;


void main()
{
    vec3 D = fPos - lights[0].position;
    normalize(D);
    vec4 DiffuseColor = max(dot(D, fNor), 0) * lights[0].color * uniformColor;

    vec4 ambientColor = lights[0].color * uniformColor * lights[0].ambientIntensity;
   FragColor = DiffuseColor + ambientColor;
}