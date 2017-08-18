package com.bn.mojing;

import static com.bn.mojing.ShaderUtil.createProgram;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import android.opengl.GLES20;

public class SphereModel 
{	
	int mProgram;			//鑷畾涔夋覆鏌撶绾跨潃鑹插櫒绋嬪簭id
    int muMVPMatrixHandle;	//鎬诲彉鎹㈢煩闃靛紩鐢�
    int maPositionHandle; 	//椤剁偣浣嶇疆灞炴�у紩鐢�
    int maTexCoorHandle; 	//椤剁偣绾圭悊鍧愭爣灞炴�у紩鐢�
    
    //int vertexbuffer;//VBO
   // int uvbuffer;
   // int indexbuffer;//IBO
	final int[] vertexbuffer = new int[1];
    final int[] uvbuffer = new int[1];
	final int[] indexbuffer = new int[1];


	FloatBuffer   mVertexBuffer;//椤剁偣鍧愭爣鏁版嵁缂撳啿
	FloatBuffer   mTexCoorBuffer;//椤剁偣绾圭悊鍧愭爣鏁版嵁缂撳啿
	ShortBuffer indexBuffer;

    float xAngle=0;//缁晉杞存棆杞殑瑙掑害
    float yAngle=0;//缁晊杞存棆杞殑瑙掑害
    float zAngle=0;//缁晍杞存棆杞殑瑙掑害 
       
    static double NV_PI = 3.14159265358979323846;
    int iCount=0;      
    int vCount = 0;
    public SphereModel(float radius)
    {
    	//璋冪敤鍒濆鍖栭《鐐规暟鎹殑initVertexData鏂规硶
    	creatSphere(radius);
    	
    	String vertexShader = 
    			"uniform mat4 uMVPMatrix;" +
    			"attribute vec3 aPosition;" +
    			"attribute vec2 aTexCoor;" +
    			"varying vec2 vTextureCoord;" +
    			"void main()" +     
    			"{" +  			                          	
    			"   gl_Position = uMVPMatrix * vec4(aPosition,1);" +
    			"   vTextureCoord = aTexCoor;" +
    			"}";
    			
    			String fragmentShader =			
    			"precision mediump float;" +
    			"uniform sampler2D sTexture;" +
    			"varying vec2 vTextureCoord;" +
    			"void main()" +                         
    			"{" +
    			"   vec4 finalColor=texture2D(sTexture, vTextureCoord);" +
    			"   gl_FragColor = finalColor;" +
    			"}";
    			
    	//璋冪敤鍒濆鍖栫潃鑹插櫒鐨刬ntShader鏂规硶
    	initShader(vertexShader,fragmentShader);
    }

    public void creatSphere(float radius) {
    	int segmentCount = 8;
		// int segmentCount = 15;
        int hozSegmentCount = segmentCount * 4;
        int verSegmentCount = segmentCount * 2;
        
        // cos(theta) and sin(theta) in z-x plane
        ArrayList<Float> cosTheta = new ArrayList<Float>();
        ArrayList<Float> sinTheta = new ArrayList<Float>();
        
        double theta = NV_PI / 2;
        double thetaStep = NV_PI / (segmentCount * 2);
        for (int i = 0; i < hozSegmentCount; i++, theta += thetaStep) {
            cosTheta.add((float)Math.cos(theta));
            sinTheta.add((float)Math.sin(theta));
        }
        cosTheta.add(cosTheta.get(0));
        sinTheta.add(sinTheta.get(0));
        
        // Angle in x-y plane
        double angle = (NV_PI / 2);
        double angleStep = NV_PI / verSegmentCount;
        
        // Save vertex data
        ArrayList<Float> vertexPos = new ArrayList<Float>();        
        // Save texture data
        ArrayList<Float> vertexTexCoord = new ArrayList<Float>();
        // Save normal data
        ArrayList<Float> vertexNormal = new ArrayList<Float>();
        
        for (int i = 0; i <= verSegmentCount; i++, angle -= angleStep) {
            float t = (float)i / verSegmentCount;
            double radiusInCrossSection;
            float y;

            if (i == 0) {
                radiusInCrossSection = 0;
                y = (float)radius;
            } else if (i == verSegmentCount) {
                radiusInCrossSection = 0;
                y = (float)-radius;
            } else {
                radiusInCrossSection = radius * Math.cos(angle);
                y = (float)(radius * Math.sin(angle));
            }

            for (int j = 0; j <= hozSegmentCount; j++) {
                float s = (float)(hozSegmentCount - j) / hozSegmentCount;
                vertexPos.add((float)(radiusInCrossSection * sinTheta.get(j)));
                vertexPos.add(y);
                vertexPos.add((float)(radiusInCrossSection * cosTheta.get(j)));

                vertexTexCoord.add(s);
                vertexTexCoord.add(t);
            }
        }
        
        float vertices [] = new float[vertexPos.size()];  
        for (int i = 0; i < vertexPos.size(); i++) {  
            vertices[i]=vertexPos.get(i);  
        }            
        
        float textures [] = new float[vertexTexCoord.size()];  
        for (int i = 0; i < vertexTexCoord.size(); i++) {  
        	textures[i]=vertexTexCoord.get(i);  
        }
     
        //椤剁偣鍧愭爣鏁版嵁鍒濆鍖�
		vCount = vertices.length*4;
      	ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);//鍒涘缓椤剁偣鍧愭爣鏁版嵁缂撳啿
        vbb.order(ByteOrder.nativeOrder());//璁剧疆瀛楄妭椤哄簭涓烘湰鍦版搷浣滅郴缁熼『搴�
        mVertexBuffer = vbb.asFloatBuffer();//杞崲涓篺loat鍨嬬紦鍐�
        mVertexBuffer.put(vertices);//鍚戠紦鍐插尯涓斁鍏ラ《鐐瑰潗鏍囨暟鎹�
        mVertexBuffer.position(0);//璁剧疆缂撳啿鍖鸿捣濮嬩綅缃�
		        
        //st鍧愭爣鏁版嵁鍒濆鍖�		
        ByteBuffer tbb = ByteBuffer.allocateDirect(textures.length*4);//鍒涘缓椤剁偣绾圭悊鏁版嵁缂撳啿
        tbb.order(ByteOrder.nativeOrder());//璁剧疆瀛楄妭椤哄簭涓烘湰鍦版搷浣滅郴缁熼『搴�
        mTexCoorBuffer = tbb.asFloatBuffer();//杞崲涓篺loat鍨嬬紦鍐�
        mTexCoorBuffer.put(textures);//鍚戠紦鍐插尯涓斁鍏ラ《鐐圭汗鐞嗘暟鎹�
        mTexCoorBuffer.position(0);//璁剧疆缂撳啿鍖鸿捣濮嬩綅缃�
        
        //
        // Fill indices buffer
        //
        ArrayList<Integer> alIndex = new ArrayList<Integer>();
        
        for (int row = 0; row < verSegmentCount; row++) {
            for (int col = 0; col < hozSegmentCount; col++) {
                int N10 = (int)((row + 1) * (hozSegmentCount + 1) + col);
                int N00 = (int)(row * (hozSegmentCount + 1) + col);
                
				alIndex.add(N00);
                alIndex.add(N10 + 1);
				alIndex.add(N10);
                

				alIndex.add(N00);
                alIndex.add(N00 + 1);
				alIndex.add(N10 + 1);
				
            }
        }
        
        iCount=alIndex.size();  
        short indices []=new short[iCount];  
        for (int i = 0; i < iCount; i++) {  
            indices[i]=alIndex.get(i).shortValue()  ;
        }  
        
        ByteBuffer ibb =  ByteBuffer.allocateDirect(iCount*2);
        
        ibb.order(ByteOrder.nativeOrder());
        indexBuffer=ibb .asShortBuffer();
        indexBuffer.put(indices);  
        indexBuffer.position(0); 
    }   

    //鑷畾涔夊垵濮嬪寲鐫�鑹插櫒initShader鏂规硶
    public void initShader(String vertexShader, String fragmentShader)
    {
        //鍩轰簬椤剁偣鐫�鑹插櫒涓庣墖鍏冪潃鑹插櫒鍒涘缓绋嬪簭
        mProgram = createProgram(vertexShader, fragmentShader);
        //鑾峰彇绋嬪簭涓《鐐逛綅缃睘鎬у紩鐢╥d  
        maPositionHandle = GLES20.glGetAttribLocation(mProgram, "aPosition");
        //鑾峰彇绋嬪簭涓《鐐圭汗鐞嗗潗鏍囧睘鎬у紩鐢╥d  
        maTexCoorHandle= GLES20.glGetAttribLocation(mProgram, "aTexCoor");
        //鑾峰彇绋嬪簭涓�诲彉鎹㈢煩闃靛紩鐢╥d
        muMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");   
        
    }
    public void  CreateVBO(){
       // ======== 创建并绑定VBO
        GLES20.glGenBuffers(1, vertexbuffer, 0);
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vertexbuffer[0]);
        GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, mVertexBuffer.capacity() * 4, mVertexBuffer, GLES20.GL_STATIC_DRAW);
//        
        GLES20.glGenBuffers(1, uvbuffer,0);
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, uvbuffer[0]);
        GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER, mTexCoorBuffer.capacity() * 4, mTexCoorBuffer , GLES20.GL_STATIC_DRAW);
        
//        
//     // ========= 创建并绑定IBO
        GLES20.glGenBuffers(1, indexbuffer,0);
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, indexbuffer[0]);
        GLES20.glBufferData(GLES20.GL_ELEMENT_ARRAY_BUFFER, indexBuffer.capacity() * 2, indexBuffer, GLES20.GL_STATIC_DRAW);
        

	    GLES20.glEnableVertexAttribArray( maPositionHandle );
	     GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vertexbuffer[0]);
	    //浼犻�侀《鐐逛綅缃暟鎹�
        GLES20.glVertexAttribPointer  
        (
        		maPositionHandle,   
        		3, 
        		GLES20.GL_FLOAT, 
        		false,
               0,   
               0
               );    
        //浼犻�侀《鐐圭汗鐞嗗潗鏍囨暟鎹�
        GLES20.glEnableVertexAttribArray( maTexCoorHandle );
   
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, uvbuffer[0]);
        GLES20.glVertexAttribPointer  
        (
       		maTexCoorHandle, 
        		2, 
        		GLES20.GL_FLOAT, 
        		false,
                0,   
                0  
        ); 
        
	    
        GLES20.glDisableVertexAttribArray( maPositionHandle );
 	    GLES20.glDisableVertexAttribArray( maTexCoorHandle );
    }
	public void EndVBO(){
	   GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
	   GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
       GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0);

       // int[] buffers = {vertexbuffer, uvbuffer,indexbuffer};
    GLES20.glDeleteBuffers(1, vertexbuffer, 0);
    GLES20.glDeleteBuffers(1, uvbuffer, 0);
	GLES20.glDeleteBuffers(1, indexbuffer, 0);
    }

    
    public void Draw(int texId){
    	
    	//GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
    	
    	 MatrixState.rotate(xAngle, 1, 0, 0);
    	 MatrixState.rotate(yAngle, 0, 1, 0);
    	 MatrixState.rotate(zAngle, 0, 0, 1);
    	
    	 //鍒跺畾浣跨敤鏌愬shader绋嬪簭
    	 GLES20.glUseProgram(mProgram);        
         
         //灏嗘渶缁堝彉鎹㈢煩闃典紶鍏hader绋嬪簭
         GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0);
         
         GLES20.glUniform1i(GLES20.glGetUniformLocation(mProgram, "sTexture"), 0);

         GLES20.glEnableVertexAttribArray( maPositionHandle );
	     GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vertexbuffer[0]);
	    //浼犻�侀《鐐逛綅缃暟鎹�
        GLES20.glVertexAttribPointer  
        (
        		maPositionHandle,   
        		3, 
        		GLES20.GL_FLOAT, 
        		false,
               0,   
               0
               );    
        //浼犻�侀《鐐圭汗鐞嗗潗鏍囨暟鎹�
        GLES20.glEnableVertexAttribArray( maTexCoorHandle );
   
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, uvbuffer[0]);
        GLES20.glVertexAttribPointer  
        (
       		maTexCoorHandle, 
        		2, 
        		GLES20.GL_FLOAT, 
        		false,
                0,   
                0  
        ); 
        
 	    
 	      //缁戝畾绾圭悊
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texId);
        
        GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, indexbuffer[0]);
        
 	    GLES20.glDrawElements(GLES20.GL_TRIANGLES,  iCount, GLES20.GL_UNSIGNED_SHORT,0 );

 	    
 		 GLES20.glUseProgram(0);  
 		 GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
 	     GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0 );
 	    
 	    GLES20.glDisableVertexAttribArray(maPositionHandle );
 	    GLES20.glDisableVertexAttribArray( maTexCoorHandle );
    }
    public void drawSelf(int texId)
    {
    	 MatrixState.rotate(xAngle, 1, 0, 0);
    	 MatrixState.rotate(yAngle, 0, 1, 0);
    	 MatrixState.rotate(zAngle, 0, 0, 1);
    	
    	 //鍒跺畾浣跨敤鏌愬shader绋嬪簭
    	 GLES20.glUseProgram(mProgram);        
         
         //灏嗘渶缁堝彉鎹㈢煩闃典紶鍏hader绋嬪簭
         GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0);
         
         
         //浼犻�侀《鐐逛綅缃暟鎹�
         GLES20.glVertexAttribPointer  
         (
         		maPositionHandle,   
         		3, 
         		GLES20.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer
         );       
         //浼犻�侀《鐐圭汗鐞嗗潗鏍囨暟鎹�
         GLES20.glVertexAttribPointer  
         (
        		maTexCoorHandle, 
         		2, 
         		GLES20.GL_FLOAT, 
         		false,
                2*4,   
                mTexCoorBuffer
         ); 
         
         //鍚敤椤剁偣浣嶇疆鏁版嵁
         GLES20.glEnableVertexAttribArray(maPositionHandle);
         //鍚敤椤剁偣绾圭悊鏁版嵁
         GLES20.glEnableVertexAttribArray(maTexCoorHandle);         
         
         //缁戝畾绾圭悊
         GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
         GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texId);
         
         //缁樺埗绾圭悊鐭╁舰
         GLES20.glDrawElements(  
        		 GLES20.GL_TRIANGLES,  //浠ヤ笁瑙掑舰鐨勬柟寮忓～鍏�  
                 iCount, GLES20.GL_UNSIGNED_SHORT, indexBuffer);
    }
}
