package com.baofeng.mojing.MojingTextureBaker.util;

import android.opengl.GLES20;
import android.opengl.GLES30;
import android.util.Log;

/**
 * Created by zhangxingang on 2017/9/15.
 */

public class ShaderHelper {
    private final static String TAG = "ShaderHelper";

    /**
     * Loads and compiles a vertex shader, returning the OpenGL object ID.
     */
    public static int compileVertexShader(String shaderCode) {
        return compileShader(GLES30.GL_VERTEX_SHADER, shaderCode);
    }

    /**
     * Loads and compiles a fragment shader, returning the OpenGL object ID.
     */
    public static int compileFragmentShader(String shaderCode) {
        return compileShader(GLES30.GL_FRAGMENT_SHADER, shaderCode);
    }

    /**
     * Compiles a shader, returning the OpenGL object ID.
     */
    private static int compileShader(int type, String shaderCode) {
        final int shaderObjectId = GLES30.glCreateShader(type);
        if (0 == shaderObjectId){
            if (LoggerConfig.ON){
                Log.w(TAG, "compileShader:Could not create new shader.");
                return 0;
            }
        }
        // Pass in the shader source.
        GLES30.glShaderSource(shaderObjectId, shaderCode);
        // Compile the shader.
        GLES30.glCompileShader(shaderObjectId);

        // Get the compilation status.
        final int[] compileStatus = new int[1];
        GLES30.glGetShaderiv(shaderObjectId, GLES30.GL_COMPILE_STATUS ,compileStatus, 0);

        if (LoggerConfig.ON) {
            // Print the shader info log to the Android log output.
            Log.v(TAG, "Results of compiling source:" + "\n" + shaderCode
                    + "\n:" + GLES30.glGetShaderInfoLog(shaderObjectId));
        }

        // Verify the compile status.
        if (compileStatus[0] == 0) {
            // If it failed, delete the shader object.
            GLES30.glDeleteShader(shaderObjectId);

            if (LoggerConfig.ON) {
                Log.w(TAG, "Compilation of shader failed.");
            }

            return 0;
        }
        return shaderObjectId;
    }

    private static int linkProgram(int vertexShader, int fragmentShader){
        // Create a new program object.
        final int programObjectId = GLES30.glCreateProgram();
        if (programObjectId == 0) {
            if (LoggerConfig.ON) {
                Log.w(TAG, "linkProgram: Could not create new program");
            }

            return 0;
        }

        // Attach the vertex shader to the program.
        GLES30.glAttachShader(programObjectId, vertexShader);
        // Attach the fragment shader to the program.
        GLES30.glAttachShader(programObjectId, fragmentShader);

        // Link the two shaders together into a program.
        GLES30.glLinkProgram(programObjectId);
        // Get the link status.
        final int[] linkStatus = new int[1];
        GLES30.glGetProgramiv(programObjectId, GLES30.GL_LINK_STATUS, linkStatus, 0);

        if (LoggerConfig.ON) {
            // Print the program info log to the Android log output.
            Log.v(TAG, "Results of linking program:\n" + GLES30.glGetProgramInfoLog(programObjectId));
        }
        // Verify the link status.
        if (linkStatus[0] == 0) {
            // If it failed, delete the program object.
            GLES30.glDeleteProgram(programObjectId);

            if (LoggerConfig.ON) {
                Log.w(TAG, "Linking of program failed.");
            }

            return 0;
        }
        return programObjectId;
    }
    /**
     * Validates an OpenGL program. Should only be called when developing the
     * application.
     */
    public static boolean validateProgram(int programObjectId) {
        GLES30.glValidateProgram(programObjectId);
        final int[] validateStatus = new int[1];
        GLES30.glGetProgramiv(programObjectId, GLES30.GL_VALIDATE_STATUS,
                validateStatus, 0);
        Log.v(TAG, "Results of validating program: " + validateStatus[0]
                + "\nLog:" + GLES30.glGetProgramInfoLog(programObjectId));

        return validateStatus[0] != 0;
    }

    /**
     * Helper function that compiles the shaders, links and validates the
     * program, returning the program ID.
     */
    public static int buildProgram(String vertexShaderSource,
                                   String fragmentShaderSource) {
        int program;

        // Compile the shaders.
        int vertexShader = compileVertexShader(vertexShaderSource);
        int fragmentShader = compileFragmentShader(fragmentShaderSource);

        // Link them into a shader program.
        program = linkProgram(vertexShader, fragmentShader);

        if (LoggerConfig.ON) {
            validateProgram(program);
        }

        return program;
    }
}
