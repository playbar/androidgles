package com.android.gltest;

public class GLScript {
	 
    
    public GLScript() { 
    } 
     
    public static final String vertex3 =
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
      
   public static final String fragment3 = "#version 300 es \n" +
            "precision mediump float;\n" 
           + "in vec2 vTextureCoord;\n" 
           + "uniform sampler2D sTexture;\n" 
           + "out vec4 v_color;\n"
           + "void main() { \n" 
           + 	"vec2 coord =  vTextureCoord;\n" 
           + 	"v_color = texture(sTexture, coord); \n" 
           + "}\n" 
           ; 
 
}
