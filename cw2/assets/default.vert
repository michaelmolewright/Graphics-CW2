#version 430

layout( location = 0 ) in vec3 iPosition;
layout( location = 1 ) in vec3 iNormal;

layout( location = 0 ) uniform mat4 uProjCameraWorld; 
layout( location = 3 ) uniform vec3 uColor;
layout( location = 4 ) uniform mat4 uModel;

out vec3 v2fColor;
out vec3 normal;
out vec3 fragPos;

void main()
{
    //uniform colour cube
    v2fColor = uColor;

    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
    fragPos = vec3( uModel * vec4(iPosition, 1.0) );
    // CALCULATE THIS OUTSIDE OF SHADER AND PASS THROUGH - mat3(transpose(inverse( uModel )))
    normal = mat3(transpose(inverse( uModel ))) * iNormal;
}
