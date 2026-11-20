#version 400 core
out vec4 FragColor;
in vec2 uv;
uniform sampler2D scene;
uniform int type;


vec3 GreenEverything(vec4 color){
    return color.rgb * vec3(0.4f, 1, 0.4f);
}

vec3 InvertColor(vec4 color){
    return vec3(1 - color.r, 1 - color.g, 1 - color.b);
}

vec3 EdgeDetection(){
    float offset = 0.001;
    vec2 offsets[9] = vec2[ ](
    vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
    vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2( offset, 0.0f),
    vec2(-offset, -offset), vec2(0.0f, -offset), vec2( offset, -offset)
    );
    float kernel[9] = float[ ](
    1.0, 1.0, 1.0,
    1.0, -12.0, 1.0,
    1.0, 1.0, 1.0
    );
    vec4 sampleTex[9];
    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += texture(scene, uv.st + offsets[i]) * kernel[i];
    }
    col.a = 1.0;
    return col.rgb;
}

vec3 Blur(){
    float offset = 0.001;
    vec2 offsets[9] = vec2[ ](
    vec2(-offset, offset), vec2(0.0f, offset), vec2(offset, offset),
    vec2(-offset, 0.0f), vec2(0.0f, 0.0f), vec2( offset, 0.0f),
    vec2(-offset, -offset), vec2(0.0f, -offset), vec2( offset, -offset)
    );
    float kernel[9] = float[ ](
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
    );
    vec4 sampleTex[9];
    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
    {
        kernel[i] = kernel[i] / 24;
        col += texture(scene, uv.st + offsets[i]) * kernel[i];
    }
    col.a = 1.0;
    return col.rgb;
}

void main()
{
    vec4 diffuse = texture(scene, uv);
    switch(type){
            case 0:
            FragColor = vec4(diffuse.r, diffuse.g, diffuse.b, 1);
            break;
            case 1:
            FragColor = vec4(GreenEverything(diffuse), 1);
            break;
            case 2:
            FragColor = vec4(InvertColor(diffuse), 1);
            break;
            case 3:
            FragColor = vec4(EdgeDetection(), 1);
//            FragColor = vec4(InvertColor(FragColor), 1);
            break;
            case 4:
            FragColor = vec4(Blur(), 1);
            break;
            default:
            FragColor = vec4(diffuse.r, diffuse.g, diffuse.b, 1);
            break;
    };
}