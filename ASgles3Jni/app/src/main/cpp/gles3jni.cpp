/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <EGL/egl.h>

#include "gles3jni.h"
#include "inlineHook.h"

const Vertex QUAD[4] = {
    // Square with diagonal < 2 so that it fits in a [-1 .. 1]^2 square
    // regardless of rotation.
    {{-0.7f, -0.7f}, {0x00, 0xFF, 0x00}},
    {{ 0.7f, -0.7f}, {0x00, 0x00, 0xFF}},
    {{-0.7f,  0.7f}, {0xFF, 0x00, 0x00}},
    {{ 0.7f,  0.7f}, {0xFF, 0xFF, 0xFF}},
};

const Vertex QUAD1[4] = {
    // Square with diagonal < 2 so that it fits in a [-1 .. 1]^2 square
    // regardless of rotation.
    {{-0.7f, -0.7f}, {0xff, 0xFF, 0x00}},
    {{ 0.7f, -0.7f}, {0xff, 0x00, 0xFF}},
    {{-0.7f,  0.7f}, {0xFF, 0x00, 0x00}},
    {{ 0.7f,  0.7f}, {0xFF, 0xFF, 0xFF}},
};

typedef struct
{
    GLuint programObject;
    GLint samplerLoc;
    GLuint textureId;
} UserData;

UserData gUserData;

bool checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                LOGE("Could not compile %s shader:\n%s\n",
                        shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                        infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        LOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                LOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

static void printGlString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    LOGV("GL %s: %s\n", name, v);
}


// ----------------------------


GLuint CreateSimpleTexture2D( )
{
    // Texture object handle
    GLuint textureId =0;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
            {
                    255,   0,   0, // Red
                    0, 255,   0, // Green
                    0,   0, 255, // Blue
                    255, 255,   0  // Yellow
            };

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &textureId );


    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );

    // Load the texture
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    return textureId;
}

GLuint CreateStorageTexture2D( )
{
    // Texture object handle
    GLuint textureId =0;

    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
            {
                    255,   0,   0, // Red
                    0, 255,   0, // Green
                    0,   0, 255, // Blue
                    255, 255,   0  // Yellow
            };

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &textureId );


    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );
    glTexStorage2D( GL_TEXTURE_2D, 0, GL_RGB, 2, 2 );

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, pixels );
    // Load the texture
//    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    return textureId;

}

void Init ( )
{
    char vShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec2 a_texCoord;   \n"
        "out vec2 v_texCoord;                       \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = a_position;               \n"
        "   v_texCoord = a_texCoord;                \n"
        "}                                          \n";

//   char fShaderStr[] =
//       "#version 300 es                                     \n"
//       "precision mediump float;                            \n"
//       "in vec2 v_texCoord;                                 \n"
//       "layout(location = 0) out vec4 outColor;             \n"
//       "uniform sampler2D s_texture;                        \n"
//       "void main()                                         \n"
//       "{                                                   \n"
//       "  outColor = texture( s_texture, v_texCoord );      \n"
//       "}                                                   \n";

     char fShaderStr[] =
         "#version 300 es                                     \n"
         "precision mediump float;                            \n"
         "in vec2 v_texCoord;                                 \n"
         "layout(location = 0) out vec4 outColor;             \n"
         "uniform sampler2D s_texture;                        \n"
         "vec2 brownConradyDistortion(vec2 uv)                \n"
         "{ \n"
         "   float demoScale = 1.0; \n"
         "   uv *= demoScale; \n"
         "   float barrelDistortion1 = 0.08; \n"
         "   float barrelDistortion2 = 0.0; \n"
         "   float r2 = uv.x*uv.x + uv.y*uv.y; \n"
         "   uv *= 1.0 + barrelDistortion1 * r2 + barrelDistortion2 * r2 * r2; \n"
         "   return uv;\n"
         "}\n"
         "void main()                                         \n"
         "{                                                   \n"
         "   vec2 uv = v_texCoord; \n"
         "   uv = uv * 2.0 - 1.0; \n"
         "   uv = brownConradyDistortion(uv);\n"
         "   uv = 0.5 * (uv * 1.0 + 1.0); \n"
         "   vec4 color;\n"
         "   if(uv.x>1.0||uv.y>0.93||uv.x<0.0||uv.y<0.07){\n"
         "       color = vec4(0.0,0.0,0.0,1.0);\n"
         "    }else{\n"
         "        color = texture(s_texture, uv);\n"
         "    }"
         "   outColor = color;       \n"
          "  //outColor = texture( s_texture, v_texCoord );      \n"
         "}                                                   \n";

    // Load the shaders and get a linked program object
    gUserData.programObject = createProgram ( vShaderStr, fShaderStr );

    // Get the sampler location
    gUserData.samplerLoc = glGetUniformLocation ( gUserData.programObject, "s_texture" );

    // Load the texture
//    gUserData.textureId = CreateStorageTexture2D ();
    gUserData.textureId = CreateSimpleTexture2D ();

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    return;
}


void Draw ()
{
    GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f, 0.0f,  0.0f,
                            -1.0f, 0.0f, 0.0f, 0.0f,  1.0f,
                            1.0f,  0.0f, 0.0f, 1.0f,  1.0f,
                            1.0f,  1.0f, 0.0f,  1.0f,  0.0f
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    // Set the viewport

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( gUserData.programObject );

    // Load the vertex position
    glVertexAttribPointer ( 0, 3, GL_FLOAT,
                            GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
    // Load the texture coordinate
    glVertexAttribPointer ( 1, 2, GL_FLOAT,
                            GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );

    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );

    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, gUserData.textureId );

    // Set the sampler texture unit to 0
    glUniform1i ( gUserData.samplerLoc, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

//    pthread_t seft = pthread_self();
//    char chId[32] = {0};
//    sprintf( chId, "draw thread id=%u", seft );
//    LOGI( chId );

}

// ----------------------------------------------------------------------------

Renderer::Renderer()
:   mNumInstances(0),
    mLastFrameNs(0)
{
    memset(mScale, 0, sizeof(mScale));
    memset(mAngularVelocity, 0, sizeof(mAngularVelocity));
    memset(mAngles, 0, sizeof(mAngles));
}

Renderer::~Renderer() {
}

void Renderer::resize(int w, int h) {
    float* offsets = mapOffsetBuf();
    calcSceneParams(w, h, offsets);
    unmapOffsetBuf();

    for (unsigned int i = 0; i < mNumInstances; i++) {
        mAngles[i] = drand48() * TWO_PI;
        mAngularVelocity[i] = MAX_ROT_SPEED * (2.0*drand48() - 1.0);
    }

    mLastFrameNs = 0;

    glViewport(0, 0, w, h);
}

void Renderer::calcSceneParams(unsigned int w, unsigned int h,
        float* offsets) {
    // number of cells along the larger screen dimension
    const float NCELLS_MAJOR = MAX_INSTANCES_PER_SIDE;

//    gbValue = !gbValue;
//    NCELLS_MAJOR = NCELLS_MAJOR * 2;
    // cell size in scene space
    const float CELL_SIZE = 2.0f / NCELLS_MAJOR;

    // Calculations are done in "landscape", i.e. assuming dim[0] >= dim[1].
    // Only at the end are values put in the opposite order if h > w.
    const float dim[2] = {fmaxf(w,h), fminf(w,h)};
    const float aspect[2] = {dim[0] / dim[1], dim[1] / dim[0]};
    const float scene2clip[2] = {1.0f, aspect[0]};
    const int ncells[2] = {
            (int)NCELLS_MAJOR,
            (int)floorf(NCELLS_MAJOR * aspect[1])
    };

    float centers[2][MAX_INSTANCES_PER_SIDE];
    for (int d = 0; d < 2; d++) {
        float offset = -ncells[d] / NCELLS_MAJOR; // -1.0 for d=0
        for (int i = 0; i < ncells[d]; i++) {
            centers[d][i] = scene2clip[d] * (CELL_SIZE*(i + 0.5f) + offset);
        }
    }

    int major = w >= h ? 0 : 1;
    int minor = w >= h ? 1 : 0;
    // outer product of centers[0] and centers[1]
    for (int i = 0; i < ncells[0]; i++) {
        for (int j = 0; j < ncells[1]; j++) {
            int idx = i*ncells[1] + j;
            offsets[2*idx + major] = centers[0][i];
            offsets[2*idx + minor] = centers[1][j];
        }
    }

    mNumInstances = ncells[0] * ncells[1];
    mScale[major] = 0.5f * CELL_SIZE * scene2clip[0];
    mScale[minor] = 0.5f * CELL_SIZE * scene2clip[1];
}

void Renderer::step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    uint64_t nowNs = now.tv_sec*1000000000ull + now.tv_nsec;

    if (mLastFrameNs > 0) {
        float dt = float(nowNs - mLastFrameNs) * 0.000000001f;

        for (unsigned int i = 0; i < mNumInstances; i++) {
            mAngles[i] += mAngularVelocity[i] * dt;
            if (mAngles[i] >= TWO_PI) {
                mAngles[i] -= TWO_PI;
            } else if (mAngles[i] <= -TWO_PI) {
                mAngles[i] += TWO_PI;
            }
        }

        float* transforms = mapTransformBuf();
        for (unsigned int i = 0; i < mNumInstances; i++) {
            float s = sinf(mAngles[i]);
            float c = cosf(mAngles[i]);
            transforms[4*i + 0] =  c * mScale[0];
            transforms[4*i + 1] =  s * mScale[1];
            transforms[4*i + 2] = -s * mScale[0];
            transforms[4*i + 3] =  c * mScale[1];
        }
        unmapTransformBuf();
    }

    mLastFrameNs = nowNs;
}

void Renderer::render() {
    step();

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw(mNumInstances);
    checkGlError("Renderer::render");
}

// ----------------------------------------------------------------------------

static Renderer* g_renderer = NULL;

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj);
};

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

EGLContext gShareContext;
EGLDisplay gDisplay;
EGLSurface gAuxSurface;

void createSharedContext(){
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    EGLint numConfigs = 0;
    EGLConfig config;
    int flags = 0;

    const EGLint attribList[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, 64,
        EGL_DEPTH_SIZE, 0,
        EGL_STENCIL_SIZE, 0,
        EGL_NONE
    };

    EGLint pbufferAttribs[] = {
        EGL_WIDTH, 1,
        EGL_HEIGHT, 1,
        EGL_TEXTURE_TARGET, EGL_NO_TEXTURE,
        EGL_TEXTURE_FORMAT, EGL_NO_TEXTURE,
        EGL_NONE
    };

    // Choose config
    EGLDisplay display = eglGetCurrentDisplay( );
    EGLContext context = eglGetCurrentContext();
    if ( !eglChooseConfig ( display, attribList, &config, 1, &numConfigs ) )
    {
        return;
    }

    gAuxSurface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    if(gAuxSurface == NULL) {
        return;
    }

    gShareContext = eglCreateContext( display, config, context, contextAttribs );
    return;

}

void * thread_1(void *pdata ){
    sleep( 2);
    if( !eglMakeCurrent( gDisplay, gAuxSurface, gAuxSurface, gShareContext )){
        printf("error");
    }
    gUserData.textureId = CreateSimpleTexture2D ();

//   GLuint texid;
//   glGenTextures(2, &texid );
//   printf("texid=%d", texid );
    pthread_t seft = pthread_self();
    char chId[32] = {0};
    sprintf( chId, "sub thread id=%u", seft );
    LOGI( chId );
    pthread_exit(0);
    return NULL;

}

CThreadPool gThreadPool(10);

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_init(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        delete g_renderer;
        g_renderer = NULL;
    }

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
//    if (strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) {
//        g_renderer = createES3Renderer();
//    } else if (strstr(versionStr, "OpenGL ES 2.")) {
//        g_renderer = createES2Renderer();
//    } else {
//        LOGE("Unsupported OpenGL ES version");
//    }

    g_renderer = createES2Renderer();

//    GPU_Sobel();
    Init();
    gDisplay = eglGetCurrentDisplay();
    createSharedContext();

    pthread_t id_1;
    int ret;
    ret = pthread_create( &id_1, NULL, thread_1, NULL );
    if( ret != 0 ){
        printf("Create thread error\n");
    }

//   pthread_join( id_1, NULL );

    pthread_t seft = pthread_self();
    char chId[32] = {0};
    sprintf( chId, "main thread id=%u", seft );
    LOGI( chId );

}

CMyTask taskObj;
char szTmp[] = "this is the first thread running";

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (g_renderer) {
        g_renderer->resize(width, height);
    }
    glViewport(0, 0, width, height);

    taskObj.SetData((void*)szTmp);
    gThreadPool.Create();
    gThreadPool.AddTask(&taskObj);

//    for(int i = 0; i < 5; i++)
//    {
//
//    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_GLES3JNILib_step(JNIEnv* env, jobject obj) {
    if (g_renderer) {
        g_renderer->render();
    }
//    Draw();
//    gThreadPool.AddTask(&taskObj);
}
