package com.android.gltest;

import android.opengl.GLES30;
import android.util.Log;
 
/**
 * @author gaofeng
 *
 */
public class GLHelper {
 
    /**
     *
     */
    public GLHelper() {
        // TODO Auto-generated constructor stub
    }
     
    public static int linkGL(){ 
        int programId = GLES30.glCreateProgram();//创建一个程序 
        if (programId == 0) { 
           Log.e("OPENGL", "Error Create Link Program"); 
           return 0; 
        } 
        return programId; 
    } 
       
    public static int linkAttach(int vertexsharder,int fragmentsharder){ 
        int programId = linkGL(); 
        GLES30.glAttachShader(programId, vertexsharder); //和着色器进行关联 
        GLES30.glAttachShader(programId, fragmentsharder);//和着色器进行关联 
        GLES30.glLinkProgram(programId); //把program链接起来 
        int status[] = new int[1]; 
        GLES30.glGetProgramiv(programId, GLES30.GL_LINK_STATUS, status, 0); //这地方一样是检查是否有错误发生。 
        Log.d("OPENGL","linkAttach link status is " + GLES30.glGetProgramInfoLog(programId)); 
        if (status[0] == 0) { 
            Log.e("OPENGL","link status is error."); 
            GLES30.glDeleteProgram(programId); 
            return 0; 
        } 
        return programId; 
    }
     
    public static boolean checkProgram(int programId){ 
        GLES30.glValidateProgram(programId); 
        int status[] = new int[1]; 
        GLES30.glGetProgramiv(programId,GLES30.GL_VALIDATE_STATUS, status,0); 
        if (status[0] == 0) { 
            Log.e("OPENGL","program is error"); 
            return false; 
        } 
        return true; 
    } 
     
    public static int compileScript(int type, String script){ 
        int objID  = GLES30.glCreateShader(type); //创建一个着色器对象,TYPE表示顶点着色器和片段着色器 
        if (objID == 0) { //0表示有错误 
            return 0; 
        } 
        GLES30.glShaderSource(objID, script); //把脚本代码传给OpenGL 引擎 
        GLES30.glCompileShader(objID); //开始编译 
        int[] status = new int[1]; 
        GLES30.glGetShaderiv(objID, GLES30.GL_COMPILE_STATUS, status, 0); //看看编译结果是否有错误。 
        Log.d("OPENGL","compileScript status info:" + GLES30.glGetShaderInfoLog(objID)); 
        if (status[0] == 0) { 
            GLES30.glDeleteShader(objID);//有错误我们删除这个对象。 
            Log.e("OPENGL", "Error Compile Script:" + script); 
            return 0; 
        } 
        return objID; 
    } 
 
}
