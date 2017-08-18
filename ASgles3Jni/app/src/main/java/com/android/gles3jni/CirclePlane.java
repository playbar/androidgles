package com.android.gles3jni;

import java.nio.FloatBuffer;  
import android.opengl.GLES30;  
  
  
//@TargetApi(18)  
public class CirclePlane {

    static final int COORDS_PRE_VERTEX = 3;
    private final int vertexStride = COORDS_PRE_VERTEX * 4;
    private int vertexCount = 0;

    private int mRow = 10;
    private int mColumn = 20;
    private float mRadius = 1.0f;
    private FloatBuffer mPlaneBuffer;

    private final int mProgram;
    private int mColorHandle;
    private int mMVPMatrixHandle;

    // Set color with red, green, blue and alpha (opacity) values  
    float color[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //白色不透明

    private final String vertexShaderCode =
    			"#version 300 es \n" +
            "uniform mat4 uMVPMatrix;" +
            "layout(location = 0) in vec4 vPosition;" +
            "out vec4 color;" +
            "void main()" +
            "{" +
            "   color = vec4(0.0f, 1.0f, 0.0f, 1.0f);"+
            "   if(vPosition.w>0.5f)"+
            "       color = vec4(1.0f, 0.0f, 0.0f, 1.0f);"+
            "   gl_Position = uMVPMatrix * vPosition;" +
            "}";
    private final String fragmentShaderCode =
    			"#version 300 es \n" +
            "precision mediump float;" +
            "uniform vec4 vColor;" +
            "in vec4 color;" +
            "out vec4 v_color;" +
            "void main()" +
            "{" +
            "   v_color = color;" +
            "}";  
      
    public CirclePlane(int row, int column, float radius)   
    {  
        // TODO Auto-generated constructor stub  
        this.mRow = row;  
        this.mColumn = column;  
        this.mRadius = radius;  
        this.createGraphics();  
          
        int vertexShader = GLES3View.loadShader(GLES30.GL_VERTEX_SHADER, vertexShaderCode);  
        int fragmentShader =GLES3View.loadShader(GLES30.GL_FRAGMENT_SHADER, fragmentShaderCode);  
          
        this.mProgram = GLES30.glCreateProgram();             // create empty OpenGL Program  
        GLES30.glAttachShader(this.mProgram, vertexShader);   // add the vertex shader to program  
        GLES30.glAttachShader(this.mProgram, fragmentShader); // add the fragment shader to program  
        GLES30.glLinkProgram(this.mProgram);                  // create OpenGL program executables 
        
        this.mColorHandle = GLES30.glGetUniformLocation(this.mProgram, "vColor");  
        this.mMVPMatrixHandle = GLES30.glGetUniformLocation(this.mProgram, "uMVPMatrix");  
        
    }  
    private void createGraphics()  
    {  
        Vertex3f vertexs[][] = new Vertex3f[this.mRow][this.mColumn];  
        float intervalR = this.mRadius / this.mRow;  
        Vertex3f centralPos = new Vertex3f(0.0f, 0.0f, 0.0f);  
        for(int i=0;i<this.mRow;i++)  
        {  
            float tmpR = intervalR * i;  
            for(int j=0;j<this.mColumn;j++)  
            {  
                double angle = 2 * j * Math.PI / (this.mColumn - 1);  
                vertexs[i][j] = new Vertex3f((float)(tmpR * Math.cos(angle)), (float)(tmpR * Math.sin(angle)), centralPos.z);  
            }  
        }  
          
        //创建三角形顶点  
        int len = 2 * (this.mRow -1) * (this.mColumn - 1) * 3;  
        this.vertexCount = len;  
        Vertex3f tri[] = new Vertex3f[len];  
        int index = 0;  
        for(int i=0;i<this.mRow-1;i++)  
        {  
            for(int j=0;j<this.mColumn-1;j++)  
            {  
                tri[index] = vertexs[i][j];  
                tri[index+1] = vertexs[i+1][j];  
                tri[index+2] = vertexs[i+1][j+1];  
                  
                tri[index+3] = vertexs[i][j];  
                tri[index+4] = vertexs[i+1][j+1];  
                tri[index+5] = vertexs[i+1][j];  
                  
                index += 6;  
            }  
        }  
          
        //设置顶点缓存  
        float[] plane = new float[len*3];  
        for(int i=0;i<len;i++)  
        {  
            int vertexI = 3 * i;  
            plane[vertexI] = tri[i].x;  
            plane[vertexI+1] = tri[i].y;  
            plane[vertexI+2] = 0.5f;//tri[i].z;
        }  
          
        this.mPlaneBuffer = GLESUntil.getFloatBuffer(plane);  
//      plane = null;  
    }   
    public void Draw(float[] mvpMatrix)  
    {  
        GLES30.glUseProgram(this.mProgram);  
        
        GLES30.glUniform4fv(this.mColorHandle, 1, this.color, 0);  
        GLES30.glUniformMatrix4fv(this.mMVPMatrixHandle, 1, false, mvpMatrix, 0);  
        
        GLES30.glEnableVertexAttribArray(0);
//        COORDS_PRE_VERTEX
        GLES30.glVertexAttribPointer(0, 2,
                                    GLES30.GL_FLOAT, false, this.vertexStride, this.mPlaneBuffer);  
        
        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, this.vertexCount);  
          
        GLES30.glDisableVertexAttribArray(0);  
          
 
    }  
} 

