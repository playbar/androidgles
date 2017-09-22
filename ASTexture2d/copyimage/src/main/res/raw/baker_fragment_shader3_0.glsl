#version 300 es
precision mediump float;
layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;
//layout(location = 0) out vec4 outColor;
in vec2 v0_texCoord;
in vec2 v1_texCoord;
in vec2 v2_texCoord;
uniform sampler2D s_texture;

void main() {
//    outColor = texture(s_texture, v_texCoord);
    fragData0 = texture(s_texture, v0_texCoord);
    fragData1 = texture(s_texture, v1_texCoord);
    fragData2 = texture(s_texture, v2_texCoord);
}
