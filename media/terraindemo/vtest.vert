#version 120
uniform mat4 mWorldViewProj;
varying vec4 pos;

void main(void)
{

    gl_Position = mWorldViewProj * gl_Vertex;
    pos = mWorldViewProj * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
