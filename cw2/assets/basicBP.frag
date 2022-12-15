#version 430

in vec3 normal;
in vec3 fragPos;  

layout( location = 2 ) uniform vec3 cameraPos;
// light uniforms
layout( location = 3 ) uniform vec3 lightPos;
layout( location = 4 ) uniform vec3 lightColor;
layout( location = 5 ) uniform vec3 objectColor;
layout( location = 6 ) uniform float emissive;



layout( location = 0 ) out vec4 oColor;

void main()
{
    // vec3 lightColor = vec3( 1.0 );
    // vec3 objectColor = vec3( 1.0, 0.5, 0.9 );


    vec3 lightDir = normalize( lightPos - fragPos ); 
    vec3 viewDir = normalize( cameraPos - fragPos );
    vec3 halfwayDir = normalize( lightDir + viewDir );

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse lighting
    vec3 norm = normalize( normal );
    float NdotL = max( dot(lightDir, norm), 0.0 );
    vec3 diffuse = NdotL * lightColor;


    // OUTPUT NORMALS
    //oColor = vec4( norm, 1.0 );
    //return;

    // OUTPUT LIGHTDIR
    // oColor = vec4( lightDir, 1.0 );
    // return;

    // OUTPUT DIFFUSE  
    // oColor = vec4( diffuse, 1.0 );
    // return;


    // specular lighting
    // PHONG
    // float specularStrength = 0.5;
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    // vec3 specular = specularStrength * spec * lightColor;  

    // CORRECTED
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = lightColor * spec;


    vec3 result = (ambient + diffuse + specular + emissive) * objectColor;
    oColor = vec4(result, 1.0);
}
