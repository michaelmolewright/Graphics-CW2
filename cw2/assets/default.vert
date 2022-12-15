#version 430

layout( location = 0 ) in vec3 iPosition;
layout( location = 1 ) in vec3 iNormal;
layout( location = 2 ) in vec2 texCoodin;

layout( location = 0 ) uniform mat4 uProjCameraWorld; 
layout( location = 1 ) uniform mat4 uModel;

out vec3 normal;
out vec3 fragPos;
out vec2 texCood;

void main()
{    
    normal = mat3( transpose( inverse( uModel ) )) * iNormal;
    texCood = texCoodin;

    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
    fragPos = vec3( uModel * vec4( iPosition, 1.0 ));
}

