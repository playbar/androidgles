package com.android.gles3jni;

import java.nio.ByteBuffer;  
import java.nio.ByteOrder;  
import java.nio.FloatBuffer;  
import java.nio.IntBuffer;  
 

public class GLESUntil {  
  
  
    //获取整形缓冲数据  
    public static IntBuffer getIntBuffer(int[] vertexs)  
    {  
        IntBuffer buffer;  
        ByteBuffer qbb = ByteBuffer.allocateDirect(vertexs.length * 4);  
        qbb.order(ByteOrder.nativeOrder());  
        buffer = qbb.asIntBuffer();  
        buffer.put(vertexs);  
        buffer.position(0);  
        return buffer;  
    }  
      
    //获取浮点形缓冲数据  
    public static FloatBuffer getFloatBuffer(float[] vertexs)  
    {  
        FloatBuffer buffer;  
          
        ByteBuffer qbb = ByteBuffer.allocateDirect(vertexs.length * 4);  
        qbb.order(ByteOrder.nativeOrder());  
        buffer = qbb.asFloatBuffer();  
        buffer.put(vertexs);  
        buffer.position(0);  
        return buffer;  
    }  
      
    //获取字节型缓冲数据  
    public static ByteBuffer getByteBuffer(byte[] vertexs)  
    {  
        ByteBuffer buffer = null;  
          
        buffer = ByteBuffer.allocateDirect(vertexs.length);  
        buffer.put(vertexs);  
        buffer.position(0);  
          
        return buffer;  
    }  
} 

