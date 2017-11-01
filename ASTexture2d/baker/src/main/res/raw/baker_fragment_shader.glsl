//#version 300 es
precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;
int iOutDrawBuffer0 = 0;
int iOutDrawBuffer1 = 1;
int iOutDrawBuffer2 = 2;
int iOutDrawBuffer3 = 3;
void main() {
//    gl_FragColor = texture2D(s_texture, v_texCoord);
    gl_FragData[iOutDrawBuffer0] = vec4 ( 1, 0, 0, 1 );
    int i1 = 1;
    gl_FragData[iOutDrawBuffer1] = vec4 ( 1, 0, 0, 1 );
     int i2 = 2;
    gl_FragData[iOutDrawBuffer2] = vec4 ( 1, 0, 0, 1 );
     int i3 = 3;
    gl_FragData[iOutDrawBuffer3] = vec4 ( 1, 0, 0, 1 );
}

