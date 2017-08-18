package com.bn.mojing;
import static com.bn.mojing.ShaderUtil.createProgram;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import android.opengl.GLES20;
public class SkyBoxModel {
	int mProgram;			//Shader的运行脚本ID
    int muMVPMatrixHandle;	//Shader的参数，用于MVP矩阵
    int maPositionHandle; 	//
    int maTexCoorHandle; 	//椤剁偣绾圭悊鍧愭爣灞炴�у紩鐢�
    
	final int[] vertexbuffer = new int[1];
    final int[] uvbuffer = new int[1];
	final int[] indexbuffer = new int[1];


	FloatBuffer   mVertexBuffer;
	FloatBuffer   mTexCoorBuffer;
	ShortBuffer indexBuffer;

    int miIndexCount = 36;
    public SkyBoxModel(float fBoxSize , float fSafeEdge)
    {
    	//璋冪敤鍒濆鍖栭《鐐规暟鎹殑initVertexData鏂规硶
    	creatSkyBox(fBoxSize , fSafeEdge);
    	
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
    
    public void creatSkyBox(float fBoxSize ,float fSafeEdge) 
    {// 现在只考虑一张图帖6面的做法，暂时不考虑其他的
    	/*------------------------------
    	 *|   R    |     L    |   T    |
    	 *|-----------------------------
    	 *|   BT   |     F    |   BA   |
    	 * -----------------------------
    	 */
    	// 1 六面体空间坐标
    	float fBoxHalfSize = fBoxSize / 2.0f;
    	// 所有坐标都以位于(000),面朝平面方向为准,顺时针添加
    	float []VertexBase ={	/*右面*/
								+1 , +1,  -1 ,// 右前上0
								+1 , +1,  +1 ,// 右后上1
								+1 , -1,  +1 ,// 右后下2
								+1 , -1,  -1 ,// 右前下3
								/*左面*/
								-1 , +1 , +1 ,// 左后上 4 
								-1 , +1 , -1 ,// 左前上5
								-1 , -1 , -1 ,// 左前下6
								-1 , -1 , +1 ,// 左后下7
								
								/*上面*/
								-1, +1, +1,//左后上8
								+1, +1, +1,//右后上9
								+1, +1, -1,//右前上10
								-1, +1, -1,//左前上 11
								/*下面*/
								-1, -1, -1,//左前下12
								+1, -1, -1,//右前下13
								+1, -1, +1,//右后下14
								-1, -1, +1,//左后下15
    							/*前面*/
    							-1 , 1 , -1, // 左前上 - 16
    							+1 , 1 , -1, // 右前上 - 17
    							+1 ,-1 , -1, // 右前下 - 18
    							-1 ,-1 , -1, // 左前下 - 19
    							/*后面*/
    							+1 , 1 , +1, // 右后上 - 20
    							-1 , 1 , +1, // 左后上 - 21
    							-1 ,-1 , +1,  // 左后下 - 22
    							+1 ,-1 , +1 // 右后下 - 23
    							};
    	float []Vertex = new float[VertexBase.length];
    	for (int iVertexIndex = 0 ; iVertexIndex < VertexBase.length ; iVertexIndex++)
    	{
    		Vertex[iVertexIndex] = VertexBase[iVertexIndex] * fBoxHalfSize;
    	}
    	ByteBuffer vbb = ByteBuffer.allocateDirect(6 * 12 *4);//鍒涘缓椤剁偣鍧愭爣鏁版嵁缂撳啿
        vbb.order(ByteOrder.nativeOrder());//璁剧疆瀛楄妭椤哄簭涓烘湰鍦版搷浣滅郴缁熼『搴�
        mVertexBuffer = vbb.asFloatBuffer();//杞崲涓篺loat鍨嬬紦鍐�
        mVertexBuffer.put(Vertex);//鍚戠紦鍐插尯涓斁鍏ラ《鐐瑰潗鏍囨暟鎹�
        mVertexBuffer.position(0);//璁剧疆缂撳啿鍖鸿捣濮嬩綅缃�
        
        
    	float SubImageSizeWidth = 1.0f/3.0f;
    	float SubImageSizeHeight = 1.0f/2.0f;
    	// 先假定为3X2的坐标网格
    	float UV[] = {
    			/*右面*/
    			0 , 0 , 
    			1 , 0 ,
    			1 , 1 ,
    			0 , 1 ,
				/*左面*/
				1 , 0 ,
				2 , 0 ,
				2 , 1 ,
				1 , 1 ,
				/*上面 */
				2 , 0 ,
				3 , 0 ,
				3 , 1 ,
				2 , 1 ,
				/*下面 */
				0 , 1 , 
    			1 , 1 ,
    			1 , 2 ,
    			0 , 2 ,
				/*前面*/
    			1 , 1 ,
				2 , 1 ,
				2 , 2 ,
				1 , 2 ,
				/*后面*/
    			2 , 1 ,
				3 , 1 ,
				3 , 2 ,
				2 , 2 
    	};
    	for (int Said = 0 ; Said < 6 ; Said ++)
    	{
    		int iIndexBase = Said * 4 * 2;
    		// 左上
    		UV[iIndexBase + 0] = UV[iIndexBase + 0] * SubImageSizeWidth + fSafeEdge;
    		UV[iIndexBase + 1] = UV[iIndexBase + 1] * SubImageSizeHeight+ fSafeEdge;
    		// 右上
    		UV[iIndexBase + 2] = UV[iIndexBase + 2] * SubImageSizeWidth - fSafeEdge;
    		UV[iIndexBase + 3] = UV[iIndexBase + 3] * SubImageSizeHeight+ fSafeEdge;
    		// 右下
    		UV[iIndexBase + 4] = UV[iIndexBase + 4] * SubImageSizeWidth - fSafeEdge;
    		UV[iIndexBase + 5] = UV[iIndexBase + 5] * SubImageSizeHeight- fSafeEdge;
    		// 左下
    		UV[iIndexBase + 6] = UV[iIndexBase + 6] * SubImageSizeWidth + fSafeEdge;
    		UV[iIndexBase + 7] = UV[iIndexBase + 7] * SubImageSizeHeight- fSafeEdge;
    		
    	}
    	 ByteBuffer tbb = ByteBuffer.allocateDirect(48*4);//鍒涘缓椤剁偣绾圭悊鏁版嵁缂撳啿
         tbb.order(ByteOrder.nativeOrder());//璁剧疆瀛楄妭椤哄簭涓烘湰鍦版搷浣滅郴缁熼『搴�
         mTexCoorBuffer = tbb.asFloatBuffer();//杞崲涓篺loat鍨嬬紦鍐�
         mTexCoorBuffer.put(UV);//鍚戠紦鍐插尯涓斁鍏ラ《鐐圭汗鐞嗘暟鎹�
         mTexCoorBuffer.position(0);//璁剧疆缂撳啿鍖鸿捣濮嬩綅缃�
    	// 3 六面体索引
    	short alIndex []= {
    	// 右面
    	0,2,3 ,		
		0,1,2 ,
    	// 左面
		4,6,7,
		4,5,6,
		
		// 上面
		8,10,11,
		8,9,10,
		
		// 下面
		12,14,15,
		12,13,14,
		// 前面
		16,18,19,
		16,17,18,
		// 后面
		20,22,23,
		20,21,22
    	};
    	ByteBuffer ibb =  ByteBuffer.allocateDirect(miIndexCount*2);        
        ibb.order(ByteOrder.nativeOrder());
        indexBuffer=ibb .asShortBuffer();
        indexBuffer.put(alIndex);  
        indexBuffer.position(0); 
    }
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
//      // ========= 创建并绑定IBO
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

     GLES20.glDeleteBuffers(1, vertexbuffer, 0);
     GLES20.glDeleteBuffers(1, uvbuffer, 0);
 	GLES20.glDeleteBuffers(1, indexbuffer, 0);
     }
 	
 	public void Draw(int texId){
    	
    	//GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
    	
    	 MatrixState.rotate(0, 1, 0, 0);
    	 MatrixState.rotate(0, 0, 1, 0);
    	 MatrixState.rotate(0, 0, 0, 1);
    	
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
        
 	    GLES20.glDrawElements(GLES20.GL_TRIANGLES,  miIndexCount, GLES20.GL_UNSIGNED_SHORT,0 );
 	    GLES20.glClearColor(0,0,0,1);
 	    GLES20.glDrawElements(GLES20.GL_LINES,  miIndexCount, GLES20.GL_UNSIGNED_SHORT,0 );
 	   
 	    
 		 GLES20.glUseProgram(0);  
 		 GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
 	     GLES20.glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, 0 );
 	    
 	    GLES20.glDisableVertexAttribArray(maPositionHandle );
 	    GLES20.glDisableVertexAttribArray( maTexCoorHandle );
    }
    public void drawSelf(int texId)
    {
    	 MatrixState.rotate(0, 1, 0, 0);
    	 MatrixState.rotate(0, 0, 1, 0);
    	 MatrixState.rotate(0, 0, 0, 1);
    	
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
                 miIndexCount, GLES20.GL_UNSIGNED_SHORT, indexBuffer);
    }
}
