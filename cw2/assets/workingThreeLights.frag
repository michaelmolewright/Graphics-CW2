#version 430


struct Light{
    vec3 position;
    vec3 ambient;
    vec3 color;
};

uniform Light light[5];

in vec3 normal;
in vec3 fragPos;
in vec2 texCood; 

layout( location = 2 ) uniform vec3 cameraPos;
// material uniforms
layout( location = 3 ) uniform vec3 materialAmbient;
layout( location = 4 ) uniform vec3 materialDiffuse;
layout( location = 5 ) uniform vec3 materialSpecular;
layout( location = 6 ) uniform float materialShininess;
layout( location = 7 ) uniform float materialEmissive;

layout( location = 8 ) uniform bool drawTexture;
uniform sampler2D ourTexture;




vec3 calcLights(Light light, vec3 normal, vec3 fragPos, vec3 camDir);

layout( location = 0 ) out vec4 oColor;

float PI = 3.1415926f;

void main()
{

    vec3 camDir = normalize(cameraPos - fragPos);
    vec3 result = {0.f, 0.f, 0.f};

    for(int i = 0; i < 5; i++){
        result += calcLights(light[i], normal, fragPos, camDir);
    }

    //emmisive term added after all 4 lights have been computed 
    result += materialEmissive;


    if (drawTexture){
        vec4 textureColour = texture(ourTexture, texCood);
        oColor = textureColour * vec4( result, 1.0 );
    }
    else{
        oColor = vec4( result, 1.0 );
    }

}

vec3 calcLights(Light light, vec3 normal, vec3 fragPos, vec3 camDir){
    vec3 res;
    float dist = length(light.position - fragPos);

    // inverse square law
    light.color = 35.f * (light.color / (dist * dist));

    //diffuse Prep
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    //Specular Prep
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(camDir, reflectDir), 0.0), materialShininess);
    

    vec3 diffuse, ambient, specular;

    //corrected bling phiong as shown in slides
    diffuse = (light.color / PI) * diff * materialDiffuse;
    ambient = light.ambient * materialAmbient;
    specular = ( (materialShininess + 2 ) / 8) * light.color * spec * materialSpecular;


    res = diffuse + ambient + specular;
    
    return res;
}