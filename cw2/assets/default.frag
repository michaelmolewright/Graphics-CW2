#version 430

in vec3 v2fColor;
in vec2 texCood;

//layout( location = 0 ) uniform vec3 uBaseColor;

layout( location = 0 ) out vec4 oColor;

uniform sampler2D ourTexture;

void main()
{
    oColor = texture(ourTexture, texCood);
}
