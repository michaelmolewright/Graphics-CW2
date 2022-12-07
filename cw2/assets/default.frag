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
    // ambient
    float ambientStrengh = 0.1;
    vec3 ambient = ambientStrengh * lightColor;

    // diffuse
    vec3 norm = normalize( normal );
    vec3 lightDir = normalize( lightPos - fragPos );  
    float diff = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot( viewDir, reflectDir ), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result = ( ambient + diffuse + specular ) * uColor;
    oColor = vec4( result, 1.0 );
}
