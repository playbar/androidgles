//
// Created by hou guoli on 2019/10/8.
//

//#include <pthread.h>
#include "gles3jni.h"

EGLContext gShareContext;
EGLDisplay gDisplay;
EGLSurface gAuxSurface;

class UserData
{
public:
    GLuint programObject;
    GLint samplerLoc;
    GLuint textureId;
};

UserData gUserData;


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
    gUserData.textureId = CreateSimpleTexture2D();

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

Renderer* craeteShareContextRender()
{
    ShareContextRender* renderer = new ShareContextRender;
    if (!renderer->init()) {
        delete renderer;
        return NULL;
    }
    gDisplay = eglGetCurrentDisplay();
    createSharedContext();

    pthread_t id_1;
    int ret;
    ret = pthread_create( &id_1, NULL, thread_1, NULL );
    if( ret != 0 ){
        printf("Create thread error\n");
    }
    return renderer;
}

ShareContextRender::ShareContextRender()
:programObject(0)
{
    programObject = 0;
}

ShareContextRender::~ShareContextRender()
{

}

bool ShareContextRender::init()
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

    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform sampler2D s_texture;                        \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  outColor = texture( s_texture, v_texCoord );      \n"
            "}                                                   \n";

    // Load the shaders and get a linked program object
    gUserData.programObject = createProgram ( vShaderStr, fShaderStr );

    // Get the sampler location
    gUserData.samplerLoc = glGetUniformLocation ( gUserData.programObject, "s_texture" );

    // Load the texture
//    gUserData.textureId = CreateStorageTexture2D ();
//    gUserData.textureId = CreateSimpleTexture2D ();

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );

    return true;
}

void ShareContextRender::resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ShareContextRender::render()
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
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
    // Load the texture coordinate
    glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );

    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );

    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, gUserData.textureId );

    // Set the sampler texture unit to 0
    glUniform1i ( gUserData.samplerLoc, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}

