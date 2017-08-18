package com.android.gltest;


import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
 
import android.opengl.GLES30;
import android.opengl.Matrix;

/**
 * @author gaofeng
 *
 */
public class MyDrawModel {
    private int programId; 
    private int mVPMatrixHandle;
    private FloatBuffer vertexBuffer; 
    private FloatBuffer texCoorBuffer;
    private FloatBuffer colorBuffer;
   
    public MyDrawModel() { 
    } 
   
    public void init() { 
        initData(); 
        int vertexsharder = GLHelper.compileScript(GLES30.GL_VERTEX_SHADER,  GLScript.vertex3);
        int fragmentsharder = GLHelper.compileScript(GLES30.GL_FRAGMENT_SHADER,  GLScript.fragment3);
//        int vertexsharder = GLHelper.compileScript(GLES30.GL_VERTEX_SHADER,  GLScript.vertexSrc);
//        int fragmentsharder = GLHelper.compileScript(GLES30.GL_FRAGMENT_SHADER,  GLScript.fragmentSrc);
        programId = GLHelper.linkAttach(vertexsharder, fragmentsharder); 
        boolean isOK = GLHelper.checkProgram(programId); 
        mVPMatrixHandle = GLES30.glGetUniformLocation(programId, "uMVPMatrix"); 
    } 
   
    private void initData() { 
        //X,Y,Z 顶点 
        float vertices[] = new float[] { 
        			-1f,  1f, 0,
        			1f,  1f, 0,
        			-1f, -1f, 0,
        			1f, -1f, 0,
           }; 
   
        ByteBuffer vb = ByteBuffer.allocateDirect(vertices.length * 4); 
        vb.order(ByteOrder.nativeOrder()); 
        vertexBuffer = vb.asFloatBuffer(); 
        vertexBuffer.put(vertices); 
        vertexBuffer.position(0); 
   
        //纹理空间坐标UV
        float texCoor[] = new float[] {
        			0f,     0f,
        			1f,     0f, 
        			0f,     1f, 
        			1f,     1f,
            }; 
   
        ByteBuffer cb = ByteBuffer.allocateDirect(texCoor.length * 4); 
        cb.order(ByteOrder.nativeOrder()); 
        texCoorBuffer = cb.asFloatBuffer(); 
        texCoorBuffer.put(texCoor); 
        texCoorBuffer.position(0);

        float colors[] = new float[] {
                1.0f, 0.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 1.0f,
        };

        ByteBuffer colorb = ByteBuffer.allocateDirect(colors.length * 4);
        colorb.order(ByteOrder.nativeOrder());
        colorBuffer = colorb.asFloatBuffer();
        colorBuffer.put(colors);
        colorBuffer.position(0);

    }

    float mAngle = 0.0f;
    float mDepth = 0.0f;
    public void drawFrame(int textureId) { 
           
         GLES30.glUseProgram(programId); 
            
         // // 初始化矩阵

//        mAngle += 1.0f;
//        if( mAngle >=360)
//        {
//            mAngle = 0;
//        }

//        mDepth += 0.01;
//        if( mDepth >= 1.0 )
//        {
//            mDepth = 0.0f;
//        }
         Matrix.setRotateM(GLRenderer.matrixs, 0, mAngle, 0, 0, 1);
         Matrix.translateM(GLRenderer.matrixs, 0, 0, 0, mDepth);
            
         //矩阵转换 ,投影矩阵，摄像机矩阵，模型矩阵 
         GLRenderer.mViewPjMatrix = new float[16]; 
         Matrix.multiplyMM(GLRenderer.mViewPjMatrix, 0, GLRenderer.viewMatrix,0, GLRenderer.matrixs, 0); 
         Matrix.multiplyMM(GLRenderer.mViewPjMatrix, 0, GLRenderer.projMatrix,0, GLRenderer.mViewPjMatrix, 0); 
         GLES30.glUniformMatrix4fv(mVPMatrixHandle, 1, false, GLRenderer.mViewPjMatrix, 0); 
          
         GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 3 * 4, vertexBuffer); 
         GLES30.glVertexAttribPointer(1, 2, GLES30.GL_FLOAT, false, 2 * 4, texCoorBuffer);
         GLES30.glVertexAttribPointer(2, 4, GLES30.GL_FLOAT, false, 4 * 4, colorBuffer);
        GLES30.glEnableVertexAttribArray(2);

         GLES30.glEnableVertexAttribArray(0); 
         GLES30.glEnableVertexAttribArray(1);

           
         GLES30.glActiveTexture(GLES30.GL_TEXTURE0); 
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId); 
         GLES30.glDrawArrays(GLES30.GL_TRIANGLE_STRIP, 0, 4);//六个定点，绘制三角形 
   
    } 
 
}
