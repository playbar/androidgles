package com.baofeng.mojing.MojingTextureBaker.program;

import android.content.Context;
import android.opengl.GLES30;

import com.baofeng.mojing.MojingTextureBaker.R;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * Created by zhangxingang on 2017/9/15.
 */

public class SkyboxShaderProgram extends ShaderProgram {
    float[] vertexArray = {
            -1.0f, 1.0f, 1.0f,     // (0) Top-left near
            1.0f, 1.0f, 1.0f,     // (1) Top-right near
            -1.0f, -1.0f, 1.0f,     // (2) Bottom-left near
            1.0f, -1.0f, 1.0f,     // (3) Bottom-right near
            -1.0f, 1.0f, -1.0f,     // (4) Top-left far
            1.0f, 1.0f, -1.0f,     // (5) Top-right far
            -1.0f, -1.0f, -1.0f,     // (6) Bottom-left far
            1.0f, -1.0f, -1.0f      // (7) Bottom-right far
    };
//    float[] vertexArray = {
//            -0.5f, 0.5f, 0.5f,     // (0) Top-left near
//            0.5f, 0.5f, 0.5f,     // (1) Top-right near
//            -0.5f, -0.5f, 0.5f,     // (2) Bottom-left near
//            0.5f, -0.5f, 0.5f,     // (3) Bottom-right near
//            -0.5f, 0.5f, -0.5f,     // (4) Top-left far
//            0.5f, 0.5f, -0.5f,     // (5) Top-right far
//            -0.5f, -0.5f, -0.5f,     // (6) Bottom-left far
//            0.5f, -0.5f, -0.5f      // (7) Bottom-right far
//    };

    // 6 indices per cube side
    byte indexArray[] = {
            // Front
            1, 3, 0,
            0, 3, 2,

            // Back
            4, 6, 5,
            5, 6, 7,

            // Left
            0, 2, 4,
            4, 2, 6,

            // Right
            5, 7, 1,
            1, 7, 3,

            // Top
            5, 1, 4,
            4, 1, 0,

            // Bottom
            6, 2, 7,
            7, 2, 3
    };
    private FloatBuffer vertexFloatBuffer;
    private ByteBuffer indexFloatBuffer;
    private static final int POSITION_COMPONENT_COUNT = 3;
    private final int uMatrixLocation;
    private final int uTextureUnitLocation;
    private final int aPositionLocation;

    public SkyboxShaderProgram(Context context) {
        super(context, R.raw.skybox_vertex_shader, R.raw.skybox_fragment_shader);
        vertexFloatBuffer = ByteBuffer.allocateDirect(vertexArray.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(vertexArray);
        vertexFloatBuffer.position(0);
        indexFloatBuffer = ByteBuffer.allocateDirect(indexArray.length * 4)
                .put(indexArray);
        indexFloatBuffer.position(0);
        uMatrixLocation = GLES30.glGetUniformLocation(program, U_MATRIX);
        uTextureUnitLocation = GLES30.glGetUniformLocation(program, U_TEXTURE_UNIT);
        aPositionLocation = GLES30.glGetAttribLocation(program, A_POSITION);
    }

    public void setUniforms(float[] matrix, int textureId) {
        GLES30.glUniformMatrix4fv(uMatrixLocation, 1, false, matrix, 0);

        GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, textureId);
        GLES30.glUniform1i(uTextureUnitLocation, 0);

        //GLES30.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0);
        GLES30.glVertexAttribPointer(aPositionLocation, POSITION_COMPONENT_COUNT,
                GLES30.GL_FLOAT, false, 0, vertexFloatBuffer);
        GLES30.glEnableVertexAttribArray(aPositionLocation);
    }

    public void draw() {
        //GLES30.glEnable(GLES30.GL_DEPTH_TEST);
        GLES30.glEnable(GLES30.GL_CULL_FACE);
        GLES30.glCullFace(GLES30.GL_FRONT);

        //GLES30.glEnable(GLES30.GL_BLEND);
        //GLES30.glBlendFunc(GLES30.GL_ZERO, GLES30.GL_ONE);
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, 36, GLES30.GL_UNSIGNED_BYTE, indexFloatBuffer);
        GLES30.glBindTexture(GLES30.GL_TEXTURE_CUBE_MAP, 0);
        GLES30.glDisable(GLES30.GL_CULL_FACE);
    }

    public int getPositionAttributeLocation() {
        return aPositionLocation;
    }
}
