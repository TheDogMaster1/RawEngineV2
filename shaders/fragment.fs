#version 400 core
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
uniform vec4 uniformColor;
uniform sampler2D tex;
uniform float ambientIntensity;
struct Light{
    vec4 color;
    vec3 position;
};
uniform Light lights[20];

uniform int lightsAmount;


void main()
{
//    FragColor = vec4(0,0, 1,1);
    vec4 result = vec4(0, 0, 0, 1);
    for(int i = 0; i < lightsAmount; i++){
        //     vec3 direction = fPos - lights[0].position;
        vec3 direction = lights[i].position - fPos;
        //    normalize(direction);
        vec4 DiffuseColor = max(dot(normalize(fNor), normalize(direction)), 0) * lights[i].color;
        //     DiffuseColor.w = 1;

        vec4 attenuation = DiffuseColor / (1 + 1 * length(direction) + 0 * length(direction));

        vec4 albedo = texture(tex, uv);
        vec4 ambientColor = vec4(1, 1, 1, 1) * ambientIntensity;
        ambientColor.w = 1;
        result += (attenuation + ambientColor) * albedo;
    }
    FragColor = result;

//  FragColor = DiffuseColor;
//     FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}