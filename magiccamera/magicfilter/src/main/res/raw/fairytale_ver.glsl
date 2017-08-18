attribute vec4 position;
attribute vec4 inputTextureCoordinate;

varying vec2 textureCoordinate;
varying vec2 vposition;

void main()
{
    gl_Position = position;
    vposition = position.xy;
    textureCoordinate = inputTextureCoordinate.xy;
}
