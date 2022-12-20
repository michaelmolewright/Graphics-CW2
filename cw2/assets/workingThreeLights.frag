#version 430


struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light[3];

in vec3 normal;
in vec3 fragPos;
in vec2 texCood; 

layout( location = 2 ) uniform vec3 cameraPos;
// light uniforms
layout( location = 3 ) uniform vec3 lightPos;
layout( location = 4 ) uniform vec3 lightColor;
layout( location = 5 ) uniform vec3 objectColor;
layout( location = 6 ) uniform float emissive;
layout( location = 7 ) uniform float shineyness;


layout( location = 10 ) uniform bool drawTexture;
uniform sampler2D ourTexture;

layout( location = 11 ) uniform vec3 lightPosition;
layout( location = 12 ) uniform vec3 lightAmbient;
layout( location = 13 ) uniform vec3 lightDiffuse;
layout( location = 14 ) uniform vec3 lightSpecular;

layout( location = 15 ) uniform vec3 materialAmbient;
layout( location = 16 ) uniform vec3 materialDiffuse;
layout( location = 17 ) uniform vec3 materialSpecular;
layout( location = 18 ) uniform float materialShininess;


vec3 calcThreeLights(Light light, vec3 normal, vec3 fragPos, vec3 camDir);

layout( location = 0 ) out vec4 oColor;

void main()
{
    // ambient
    //vec3 ambient = light[0].ambient * materialAmbient;
  	//
    //// diffuse 
    //vec3 norm = normalize(normal);
    //vec3 lightDir = normalize(light[0].position - fragPos);
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = light[0].diffuse * (diff * materialDiffuse);
    //
    //// specular
    //vec3 viewDir = normalize(cameraPos - fragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    //vec3 specular = light[0].specular * (spec * materialSpecular);
//
    //vec3 result = ambient + diffuse + specular;


    vec3 camDir = normalize(cameraPos - fragPos);
    vec3 result;

    for(int i = 0; i < 3; i++){
        result += calcThreeLights(light[i], normal, fragPos, camDir);
    }

    if (drawTexture){
        vec4 textureColour = texture(ourTexture, texCood);
        oColor = textureColour * vec4( result, 1.0 );
    }
    else{
        oColor = vec4( result, 1.0 );
    }

}

vec3 calcThreeLights(Light light, vec3 normal, vec3 fragPos, vec3 camDir){
    vec3 res;

    //diffuse Prep
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    //Specular Prep
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(camDir, reflectDir), 0.0), materialShininess);
    
    float dist = length(light.position - fragPos);

    float balancer = 1.0 / ( 1.0 + (0.09 * dist) + (0.03 * (dist * dist) ) );

    vec3 diffuse, ambient, specular;

    //if (drawTexture){
    //    diffuse = light.diffuse * vec3(texture( (diff * materialDiffuse), texCood) );
    //    ambient = light.ambient * vec3(texture(materialAmbient, texCood));
    //    specular = light.specular * vec3(texture((spec * materialSpecular), texCood));
    //}
    //else{
    diffuse = light.diffuse * (diff * materialDiffuse);
    ambient = light.ambient * materialAmbient;
    specular = light.specular * (spec * materialSpecular);
    //}

    diffuse *= balancer;
    ambient *= balancer;
    specular *= balancer;
    
    res = diffuse + ambient + specular;
    
    return res;
}