#version 430

in vec3 normal;
in vec3 fragPos;  

layout( location = 2 ) uniform vec3 cameraPos;
// light uniforms
layout( location = 3 ) uniform vec3 lightPos;
layout( location = 4 ) uniform vec3 lightAmb;
layout( location = 5 ) uniform vec3 lightIncoming;
// material uniforms
layout( location = 6 ) uniform vec3 materialAmbient;
layout( location = 7 ) uniform vec3 materialDiffuse;
layout( location = 8 ) uniform vec3 materialSpecular;
layout( location = 9 ) uniform float materialShininess;
// emissive val
layout( location = 10 ) uniform float emissive;

layout( location = 0 ) out vec4 oColor;

void main()
{
    // CORRECTED BLINN PHONG IMPLEMENTATION
    const float pi = 3.1415926;

    // ambient lighting
    vec3 ambient = materialAmbient * lightAmb;

    // diffuse lighting
    vec3 norm = normalize( normal );

    // OUTPUT NORMALS
    // oColor = vec4( norm, 1.0 );
    // return;

    vec3 lightDir = normalize( lightPos - fragPos );  
    // OUTPUT LIGHTDIR
    // oColor = vec4( lightDir, 1.0 );
    // return;

    float NdotL = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = materialDiffuse * ( lightIncoming / pi )* NdotL;

    // OUTPUT DIFFUSE  
    //oColor = vec4( diffuse, 1.0 );
    //return;

    // specular lighting
    vec3 viewDir = normalize( cameraPos - fragPos );
    vec3 halfwayDir = normalize( lightDir + viewDir ); // blinn halfway vector
    float NdotH = max( dot( norm, halfwayDir ), 0.0);
    float specularTerm = pow( NdotH, materialShininess );
    float correctionTerm = ( materialShininess + 2 ) / 8;
    vec3 specular = correctionTerm * NdotL * materialSpecular * lightIncoming * specularTerm;  
    
    vec3 result = ambient + diffuse + specular + emissive;
    oColor = vec4( result, 1.0 );
}
