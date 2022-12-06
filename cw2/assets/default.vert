#version 430

layout( location = 0 ) in vec3 iPosition;
layout( location = 1 ) in vec3 iColor;
layout( location = 2 ) in vec2 texCoodin;

layout( location = 0 ) uniform mat4 uProjCameraWorld; 

out vec3 v2fColor;
out vec2 texCood;

void main()
{
    v2fColor = iColor;
    texCood = texCoodin;

    //gl_Position = vec4( iPosition.xyz, 1.0 );
    gl_Position = uProjCameraWorld * vec4( iPosition, 1.0 );
}
