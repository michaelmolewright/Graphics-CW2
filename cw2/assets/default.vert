#version 430

layout( location = 0 ) in vec3 iPosition;
//layout( location = 1 ) in vec3 iColor;
layout( location = 2 ) in vec2 texCoodin;

layout( location = 0 ) uniform mat4 uProjCameraWorld;
layout( location = 1 ) uniform mat4 uModel; 

//out vec3 v2fColor;
out vec2 texCood;

void main()
{
    //v2fColor = vec3{0.f,1.f,1.f};
    texCood = texCoodin;

    //gl_Position = vec4( iPosition.xyz, 1.0 );
    gl_Position = uProjCameraWorld * uModel * vec4( iPosition, 1.0 );
}
