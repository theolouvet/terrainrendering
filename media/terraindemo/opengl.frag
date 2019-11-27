#version 120
uniform sampler2D myTexture;

varying float a;

void main (void)
{
    vec4 col = texture2D(myTexture, vec2(gl_TexCoord[0]));
    //col *= gl_Color;


        gl_FragColor =  col ;


}
