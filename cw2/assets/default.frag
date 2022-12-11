#version 430

in vec3 normal;
in vec3 fragPos;  

layout( location = 0 ) out vec4 oColor;

layout( location = 2 ) uniform vec3 cameraPos;

// light props
layout( location = 3 ) uniform vec3 lightPos;
layout( location = 4 ) uniform vec3 lightAmbient;
layout( location = 5 ) uniform vec3 lightDiffuse;
layout( location = 6 ) uniform vec3 lightSpecular;

// material props
layout( location = 7 ) uniform vec3 materialAmbient;
layout( location = 8 ) uniform vec3 materialDiffuse;
layout( location = 9 ) uniform vec3 materialSpecular;
layout( location = 10 ) uniform float materialShininess;

void main()
{
    // ambient lighting
    vec3 ambient = materialAmbient * lightAmbient;

    // diffuse lighting
    vec3 norm = normalize( normal );
    vec3 lightDir = normalize( lightPos - fragPos );  
    float NdotL = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = materialDiffuse * lightDiffuse * NdotL;

    // specular lighting
    vec3 viewDir = normalize( cameraPos - fragPos );
    vec3 halfwayDir = normalize( lightDir + viewDir ); // blinn halfway vector
    float NdotH = max( dot( normal, halfwayDir ), 0.0);
    float specularTerm = pow( NdotH, materialShininess );
    vec3 specular = materialSpecular * lightSpecular * specularTerm;  

    // emissive term
    float emissive = 0.1;
    
    vec3 result = ambient + diffuse + specular + emissive;
    oColor = vec4( result, 1.0 );
}
