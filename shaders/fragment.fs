#version 400 core
out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;
uniform vec4 uniformColor;
uniform sampler2D tex;
uniform float ambientIntensity;
uniform vec3 camPos;
struct Light{
    vec4 color;
    vec3 position;
    float specularIntensity;
    float type;
};
uniform Light lights[20];

uniform int lightsAmount;


void main()
{
//    FragColor = vec4(0,0, 1,1);
    vec4 result = vec4(0, 0, 0, 1);
vec4 albedo = texture(tex, uv);

    for(int i = 0; i < lightsAmount; i++){
        if (lights[i].type == 0) {
            //     vec3 direction = fPos - lights[0].position;
            vec3 direction = lights[i].position - fPos;
            //    normalize(direction);
            vec4 DiffuseColor = max(dot(normalize(fNor), normalize(direction)), 0) * lights[i].color * albedo;
            //     DiffuseColor.w = 1;

            vec4 attenuation = DiffuseColor / (1 + 0.5 * length(direction) + 0.2 * length(direction));

            vec3 reflection = reflect(normalize(direction), normalize(fNor));
            vec3 camDir = normalize(fPos - camPos);

            vec4 specular = pow(max(dot(reflection, camDir), 0), lights[i].specularIntensity) * lights[i].color;
            specular.w = 1;
            result += attenuation + specular;
        }
        else if(lights[i].type == 1){
            vec3 direction = lights[i].position;
            //    normalize(direction);
            vec4 DiffuseColor = max(dot(normalize(fNor), normalize(direction)), 0) * lights[i].color * albedo;
            //     DiffuseColor.w = 1;

            result += DiffuseColor;
        }
    }
vec4 ambientColor = vec4(1, 1, 1, 1) * ambientIntensity * albedo;
ambientColor.w = 1;
    FragColor = clamp(result + ambientColor, 0, 1);
//  FragColor = DiffuseColor;
//     FragColor = vec4(fNor.x, fNor.y, fNor.z, 1);
}