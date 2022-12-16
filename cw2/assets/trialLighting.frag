#version 430



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




layout( location = 0 ) out vec4 oColor;

void main()
{
    // ambient
    vec3 ambient = lightAmbient * materialAmbient;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * (diff * materialDiffuse);
    
    // specular
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = lightSpecular * (spec * materialSpecular);

    vec3 result = ambient + diffuse + specular;

    if (drawTexture){
        vec4 textureColour = texture(ourTexture, texCood);
        oColor = textureColour * vec4( result, 1.0 );
    }
    else{
        oColor = vec4( result, 1.0 );
    }

}