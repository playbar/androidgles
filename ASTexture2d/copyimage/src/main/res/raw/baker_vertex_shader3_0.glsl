#version 300 es
//uniform mat4 u_Matrix;
//attribute vec4 vPosition;
//attribute vec2 a_texCoord;
//varying vec2 v_texCoord;
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 a0_texCoord;
layout(location = 2) in vec2 a1_texCoord;
layout(location = 3) in vec2 a2_texCoord;
out vec2 v0_texCoord;
out vec2 v1_texCoord;
out vec2 v2_texCoord;


void main() {
    gl_Position = vPosition;
    v0_texCoord = a0_texCoord;
    v1_texCoord = a1_texCoord;
    v2_texCoord = a2_texCoord;
}