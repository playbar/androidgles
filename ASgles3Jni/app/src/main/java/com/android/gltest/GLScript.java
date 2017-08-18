package com.android.gltest;

public class GLScript {
	 
    
    public GLScript() { 
    } 
     
    public static final String vertex3 =
            "#version 300 es \n" +
            "uniform mat4 uMVPMatrix;\n" 
            + "layout(location = 0) in vec3 aPosition;\n" 
            + "layout(location = 1) in vec2 aTexCoor;\n"
            + "layout(location = 2) in vec4 aColor; \n"
            + "out vec4 vColor; \n"
            + "out vec2 vTextureCoord;\n"
            + "void main() { \n" 
            + "gl_Position = uMVPMatrix * vec4(aPosition,1);\n"
            + "vColor = aColor;"
            + "vTextureCoord = aTexCoor;\n"  
            + "}\n" 
           ; 
      
   public static final String fragment3 = "#version 300 es \n" +
            "precision mediump float;\n" 
           + "in vec2 vTextureCoord;\n"
           + "in vec4 vColor; \n"
           + "uniform sampler2D sTexture;\n" 
           + "out vec4 v_color;\n"
           + "void main() { \n" 
           + 	"vec2 coord =  vTextureCoord;\n" 
           + 	"vec4 color = texture(sTexture, coord); \n"
           +    "if(coord.x > 0.995 || coord.x < 0.005 || coord.y < 0.005 || coord.y > 0.995 )"
           +    "   v_color = vColor * color.a;"
           +    "else"
           +    "   v_color = vec4(color.rgb, color.a);"
           + "}\n"
           ;

    ///////////////////////

    public static final String vertexSrc =
            "#version 300 es \n" +
                    "uniform mat4 uMVPMatrix;\n"
                    + "layout(location = 0) in vec3 aPosition;\n"
                    + "layout(location = 1) in vec2 aTexCoor;\n"
                    + "out vec2 vTextureCoord;\n"
                    + "void main() { \n"
                    + "gl_Position = uMVPMatrix * vec4(aPosition,1);\n"
                    + "vTextureCoord = aTexCoor;\n"
                    + "}\n"
            ;

    public static final String fragmentSrc = "#version 300 es \n" +
            "precision mediump float;\n"
            + "in vec2 vTextureCoord;\n"
            + "uniform sampler2D sTexture;\n"
            + "out vec4 v_color;\n"
            + "void main() { \n"
            + 	"vec2 coord =  vTextureCoord;\n"
            + 	"vec4 color = texture(sTexture, coord); \n"
//           +    "if(color.a ==1.0f) discard;"
            +    "v_color = vec4(color.rgb, color.a);"
            + "}\n"
            ;

}
