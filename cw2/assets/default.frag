#version 430

in vec3 v2fColor;
in vec3 normal;
in vec3 fragPos;  

// lighting
layout( location = 0 ) out vec4 oColor;
layout( location = 1 ) uniform vec3 lightColor;
layout( location = 2 ) uniform vec3 lightPos;

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
    
    vec3 result = ( ambient + diffuse ) * v2fColor;
    oColor = vec4( result, 1.0 );
}
