#version 430

in vec3 normal;
in vec3 fragPos;  

// lighting
layout( location = 0 ) out vec4 oColor;

layout( location = 1 ) uniform vec3 lightColor;
layout( location = 2 ) uniform vec3 lightPos;
layout( location = 3 ) uniform vec3 uColor;
layout( location = 5 ) uniform vec3 viewPos;

struct MaterialProps {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform MaterialProps material;

void main()
{
    // ambient lighting
    float ambientStrengh = 0.2;
    vec3 ambient = lightColor * ambientStrengh * material.ambient;

    // diffuse lighting
    vec3 norm = normalize( normal );
    vec3 lightDir = normalize( lightPos - fragPos );  
    float diff = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = diff * lightColor * material.diffuse;

    //specular lighting
    vec3 viewDir = normalize( viewPos - fragPos );
    // blinn halfway vector
    vec3 halfwayDir = normalize( lightDir + viewDir );
    float clampedDotProduct = max( dot( normal, halfwayDir ), 0.0);
    float spec = pow( clampedDotProduct, material.shininess );
    vec3 specular = spec * lightColor * material.specular;  
    
    vec3 result = ( ambient + diffuse + specular ) * uColor;
    oColor = vec4( result, 1.0 );
}
