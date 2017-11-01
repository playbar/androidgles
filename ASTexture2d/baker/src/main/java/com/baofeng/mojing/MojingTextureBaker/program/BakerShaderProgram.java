package com.baofeng.mojing.MojingTextureBaker.program;

import android.content.Context;
import android.graphics.PointF;
import android.opengl.GLES30;

import com.baofeng.mojing.MojingTextureBaker.data.TextureInfo;
import com.baofeng.mojing.MojingTextureBaker.util.JasonHelper;
import com.mapleaf.testbaker.R;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.List;

/**
 * Created by zhangxingang on 2017/9/20.
 */

public class BakerShaderProgram extends ShaderProgram {
    private static final int MAP_DIVISION_SIZE = 15;
    private static final int VERTEX_ARRAY_SIZE = 8;
    private static final int UNIT_VERTEX_ARRAY_SIZE = 8;
    private static final int MAP_DIVISION_X = 3;
    private static final int MAP_DIVISION_Y = 5;
    private static final float VIEWPORT_DIVISION_X = 2.0f / MAP_DIVISION_X;
    private static final float VIEWPORT_DIVISION_Y = 2.0f / MAP_DIVISION_Y;
//    float fVertexArray[] = {
//            -1.0f, 1.0f, // top left
//            1.0f, 1.0f, // top right
//            -1.0f, -1.0f,// bottom left
//            1.0f, -1.0f// bottom right
//    };
//    float fVertexArray[] = {
//            -1.0f, 1.0f, // top left
//            -1.0f + 2.0f / 3, 1.0f, // top right
//            -1.0f, 1.0f - 2.0f / 3,// bottom left
//            -1.0f + 2.0f / 3, 1.0f - 2.0f / 3// bottom right
//    };
//    float fColorVertexArray[] = {
//            0, 0,  // bottom left
//            1, 0,  // bottom right
//            0, 1,  // top left
//            1, 1,  // top right
//    };
    private short iIndexArray[] = {
            0, 2, 1, 2, 3, 1
    };

    private PointF[] fpCubemapFaceVertex;//begin vertex
    private FloatBuffer vertexFloatBuffer;
    private FloatBuffer colorVertexFloatBuffer;
    private ShortBuffer indexShortBuffer;
    private static final int POSITION_COMPONENT_COUNT = 3;
    private final int uMatrixLocation;
    private final int uTextureUnitLocation;
    private final int vPositionLocation;
    private final int a0TexCoordLocation;
    private final int a1TexCoordLocation;
    private final int a2TexCoordLocation;


    public BakerShaderProgram(Context context) {
        super(context, R.raw.baker_vertex_shader3_0, R.raw.baker_fragment_shader3_0);
        uMatrixLocation = GLES30.glGetUniformLocation(program, U_MATRIX);
        uTextureUnitLocation = GLES30.glGetUniformLocation(program, S_TEXTURE);
        vPositionLocation = GLES30.glGetAttribLocation(program, V_POSITION);
        a0TexCoordLocation = GLES30.glGetAttribLocation(program, A0_TEXCOORD);
        a1TexCoordLocation = GLES30.glGetAttribLocation(program, A1_TEXCOORD);
        a2TexCoordLocation = GLES30.glGetAttribLocation(program, A2_TEXCOORD);
    }

    public void initCubmapFaceVertexIndex()
    {
        short[] tmpIndexArray = new short[6];
        indexShortBuffer = ByteBuffer.allocateDirect(MAP_DIVISION_SIZE * iIndexArray.length * 4).order(ByteOrder.nativeOrder()).asShortBuffer();
        for (int i = 0; i < MAP_DIVISION_SIZE; i ++) {
            for (int j = 0; j < 6;j++) {
                tmpIndexArray[j] = iIndexArray[j];
                tmpIndexArray[j] += i * 4;
            }
            indexShortBuffer.put(tmpIndexArray);
        }
        indexShortBuffer.position(0);
    }

    /*
    * calculate the seperated texture coordinate from big texture
    * para:sub texture list from jason, index of current list element has been used
    * return index
    * */
    public int  initCubmapFaceFromTextureInfo(List<JasonHelper.SubTexture> mSubTexturesList, int index) {
        if (index >= mSubTexturesList.size())
        {
            return -1;
        }
        float fwidth;
        float fheight;
        PointF currentVertex = new PointF();
        int[] position;
        int[] size;
        int n = Math.min(MAP_DIVISION_SIZE * 3, mSubTexturesList.size() - index);
        int BakedTextureSize = TextureInfo.getBakedTextureSize();
        float[] tmpUnitVertexArray = new float[UNIT_VERTEX_ARRAY_SIZE];

        colorVertexFloatBuffer = ByteBuffer.allocateDirect(3 * MAP_DIVISION_SIZE * VERTEX_ARRAY_SIZE * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        for (int i = 0; i < n; i++) {
            position = mSubTexturesList.get(i + index).getPosition();
            size = mSubTexturesList.get(i + index).getSize();
            currentVertex.x = (float)position[0] / BakedTextureSize;
            currentVertex.y = (float)position[1] / BakedTextureSize;
            fwidth = (float)size[0] / BakedTextureSize;
            fheight = (float)size[1] / BakedTextureSize;
            tmpUnitVertexArray[0] = currentVertex.x;
            tmpUnitVertexArray[1] =  currentVertex.y;

            tmpUnitVertexArray[2] = currentVertex.x + fwidth;
            tmpUnitVertexArray[3] = currentVertex.y;

            tmpUnitVertexArray[4] = currentVertex.x;
            tmpUnitVertexArray[5] = currentVertex.y + fheight;

            tmpUnitVertexArray[6] = currentVertex.x + fwidth;
            tmpUnitVertexArray[7] = currentVertex.y + fheight;
            colorVertexFloatBuffer.put(tmpUnitVertexArray);
        }
        colorVertexFloatBuffer.position(0);
        return n;
    }

    public void initCubmapFaceVertex() {
        fpCubemapFaceVertex = new PointF[MAP_DIVISION_SIZE];
        for (int Y = 0; Y < MAP_DIVISION_Y; Y++)
            for (int X = 0; X < MAP_DIVISION_X; X++) {
                fpCubemapFaceVertex[Y * MAP_DIVISION_X + X] = new PointF();
                fpCubemapFaceVertex[Y * MAP_DIVISION_X + X].x = -1.0f + VIEWPORT_DIVISION_X * X;
                fpCubemapFaceVertex[Y * MAP_DIVISION_X + X].y = 1.0f - VIEWPORT_DIVISION_Y * Y;
            }
        float[] fBakerVertexArray = new float[VERTEX_ARRAY_SIZE];
        vertexFloatBuffer = ByteBuffer.allocateDirect(MAP_DIVISION_SIZE * VERTEX_ARRAY_SIZE * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        for (int i = 0; i < MAP_DIVISION_SIZE; i++) {
            fBakerVertexArray[0] = fpCubemapFaceVertex[i].x;
            fBakerVertexArray[1] = fpCubemapFaceVertex[i].y;

            fBakerVertexArray[2] = fpCubemapFaceVertex[i].x + VIEWPORT_DIVISION_X;
            fBakerVertexArray[3] = fpCubemapFaceVertex[i].y;

            fBakerVertexArray[4] = fpCubemapFaceVertex[i].x;
            fBakerVertexArray[5] = fpCubemapFaceVertex[i].y - VIEWPORT_DIVISION_Y;

            fBakerVertexArray[6] = fpCubemapFaceVertex[i].x + VIEWPORT_DIVISION_X;
            fBakerVertexArray[7] = fpCubemapFaceVertex[i].y - VIEWPORT_DIVISION_Y;
            vertexFloatBuffer.put(fBakerVertexArray);
        }
        vertexFloatBuffer.position(0);
    }

    public void setUniforms(float[] matrix, int textureId) {
        GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
        GLES30.glUniform1i(uTextureUnitLocation, 0);

        //GLES30.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0);
        vertexFloatBuffer.position(0);
        GLES30.glVertexAttribPointer(vPositionLocation, 2, GLES30.GL_FLOAT, false, 0, vertexFloatBuffer);
        GLES30.glEnableVertexAttribArray(vPositionLocation);
        // UV
        colorVertexFloatBuffer.position(0);
        GLES30.glVertexAttribPointer(a0TexCoordLocation, 2, GLES30.GL_FLOAT, false, 0, colorVertexFloatBuffer);
        GLES30.glEnableVertexAttribArray(a0TexCoordLocation);
        colorVertexFloatBuffer.position(MAP_DIVISION_SIZE * UNIT_VERTEX_ARRAY_SIZE);
        GLES30.glVertexAttribPointer(a1TexCoordLocation, 2, GLES30.GL_FLOAT, false, 0, colorVertexFloatBuffer);
        GLES30.glEnableVertexAttribArray(a1TexCoordLocation);
        colorVertexFloatBuffer.position(MAP_DIVISION_SIZE * UNIT_VERTEX_ARRAY_SIZE * 2);
        GLES30.glVertexAttribPointer(a2TexCoordLocation, 2, GLES30.GL_FLOAT, false, 0, colorVertexFloatBuffer);
        GLES30.glEnableVertexAttribArray(a2TexCoordLocation);
        indexShortBuffer.position(0);

    }

    public void draw(int textureId) {
        GLES30.glEnable(GLES30.GL_CULL_FACE);
        GLES30.glCullFace(GLES30.GL_BACK);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, 6 * MAP_DIVISION_SIZE, GLES30.GL_UNSIGNED_SHORT, indexShortBuffer);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
        GLES30.glDisable(GLES30.GL_CULL_FACE);
    }
}
