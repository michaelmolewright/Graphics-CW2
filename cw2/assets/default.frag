#version 430

in vec3 normal;
in vec3 fragPos;  

// lighting
layout( location = 0 ) out vec4 oColor;

layout( location = 1 ) uniform vec3 lightColor;
layout( location = 2 ) uniform vec3 lightPos;
layout( location = 3 ) uniform vec3 uColor;
layout( location = 5 ) uniform vec3 viewPos;

void main()
{
    // ambient lighting
    float ambientStrengh = 0.2;
    vec3 ambient = ambientStrengh * lightColor;

    // diffuse lighting
    vec3 norm = normalize( normal );
    vec3 lightDir = normalize( lightPos - fragPos );  
    float diff = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = diff * lightColor;

    //specular lighting
    float specularStrength = .6;
    float shininess = 20.0;

    vec3 viewDir = normalize( viewPos - fragPos );
    // blinn halfway vector
    vec3 halfwayDir = normalize( lightDir + viewDir );
    float clampedDotProduct = max( dot( normal, halfwayDir ), 0.0);
    float spec = pow( clampedDotProduct, shininess );
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result = ( ambient + diffuse + specular ) * uColor;
    oColor = vec4( result, 1.0 );
}
