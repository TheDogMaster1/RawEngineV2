#version 400 core
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
uniform vec4 uniformColor;
struct Light{
    vec4 color;
    vec3 position;
    float ambientIntensity;
};
uniform Light lights[20];

uniform int lightsAmount = 1;


void main()
{
//     vec3 direction = fPos - lights[0].position;
    vec3 direction = lights[0].position - fPos;
    normalize(direction);
    normalize(fNor);
    vec4 DiffuseColor = max(dot(direction, fNor), 0) * lights[0].color;
//     DiffuseColor.w = 1;

    vec4 ambientColor = lights[0].color * lights[0].ambientIntensity;
    ambientColor.w = 1;
   FragColor = DiffuseColor + ambientColor * uniformColor;
//  FragColor = DiffuseColor;
//     FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}