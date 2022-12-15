#version 430

layout( location = 0 ) in vec3 iPosition;
layout( location = 1 ) in vec3 iNormal;

layout( location = 0 ) uniform mat4 uProjCameraWorld; 
layout( location = 1 ) uniform mat4 uModel;

out vec3 normal;
out vec3 fragPos;

void main()
{    
    fragPos = vec3( uModel * vec4( iPosition, 1.0 ));

    normal = mat3( transpose( inverse( uModel ) )) * iNormal;

    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
}