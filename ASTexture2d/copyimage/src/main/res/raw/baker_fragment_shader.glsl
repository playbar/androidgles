//#version 300 es
precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;
void main() {
//    gl_FragColor = texture2D(s_texture, v_texCoord);
    gl_FragData[0] = vec4 ( 1, 0, 0, 1 );
    int i1 = 1;
    gl_FragData[i1] = vec4 ( 1, 0, 0, 1 );
     int i2 = 2;
    gl_FragData[i2] = vec4 ( 1, 0, 0, 1 );
     int i3 = 3;
    gl_FragData[i3] = vec4 ( 1, 0, 0, 1 );
}
