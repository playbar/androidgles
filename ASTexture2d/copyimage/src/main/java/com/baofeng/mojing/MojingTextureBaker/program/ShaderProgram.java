package com.baofeng.mojing.MojingTextureBaker.program;

import android.content.Context;
import android.opengl.GLES30;

import com.baofeng.mojing.MojingTextureBaker.util.ShaderHelper;
import com.baofeng.mojing.MojingTextureBaker.util.TextResourceReader;

/**
 * Created by zhangxingang on 2017/9/15.
 */

public class ShaderProgram {
    // Uniform constants
    protected static final String U_MATRIX = "u_Matrix";
    protected static final String U_COLOR = "u_Color";
    protected static final String U_TEXTURE_UNIT = "u_TextureUnit";
    protected static final String U_TIME = "u_Time";

    // Attribute constants
    protected static final String A_POSITION = "a_Position";
    protected static final String A_COLOR = "a_Color";
    protected static final String A_TEXTURE_COORDINATES = "a_TextureCoordinates";
    protected static final String A_DIRECTION_VECTOR = "a_DirectionVector";
    protected static final String A_PARTICLE_START_TIME = "a_ParticleStartTime";
    protected static final String A0_TEXCOORD = "a0_texCoord";
    protected static final String A1_TEXCOORD = "a1_texCoord";
    protected static final String A2_TEXCOORD = "a2_texCoord";
    protected static final String V_POSITION = "vPosition";
    protected static final String S_TEXTURE = "s_texture";

    // Shader program
    protected final int program;

    protected ShaderProgram(Context context, int vertexShaderResourceId,
                            int fragmentShaderResourceId) {
        // Compile the shaders and link the program.
        program = ShaderHelper.buildProgram(
                TextResourceReader
                        .readTextFileFromResource(context, vertexShaderResourceId),
                TextResourceReader
                        .readTextFileFromResource(context, fragmentShaderResourceId));
    }

    public void useProgram() {
        // Set the current OpenGL shader program to this program.
        GLES30.glUseProgram(program);
    }
}
