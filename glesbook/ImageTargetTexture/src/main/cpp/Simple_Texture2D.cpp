
#include <stdlib.h>
#include "esUtil.h"
#include "GraphicBuffer.h"
#include "android/native_window.h"
#include "my_log.h"
#include <GLES2/gl2ext.h>


typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Sampler location
   GLint samplerLoc;

   // Texture handle
   GLuint textureId;

} UserData;



EGLClientBuffer myeglCreateNativeClientBufferANDROID(const EGLint *attrib_list)
{
    int usage = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t format = 0;
    uint32_t red_size = 0;
    uint32_t green_size = 0;
    uint32_t blue_size = 0;
    uint32_t alpha_size = 0;

#define GET_NONNEGATIVE_VALUE(case_name, target) \
    case case_name: \
        if (value >= 0) { \
            target = value; \
        }\
        break

    if (attrib_list) {
        while (*attrib_list != EGL_NONE) {
            GLint attr = *attrib_list++;
            GLint value = *attrib_list++;
            switch (attr) {
                GET_NONNEGATIVE_VALUE(EGL_WIDTH, width);
                GET_NONNEGATIVE_VALUE(EGL_HEIGHT, height);
                GET_NONNEGATIVE_VALUE(EGL_RED_SIZE, red_size);
                GET_NONNEGATIVE_VALUE(EGL_GREEN_SIZE, green_size);
                GET_NONNEGATIVE_VALUE(EGL_BLUE_SIZE, blue_size);
                GET_NONNEGATIVE_VALUE(EGL_ALPHA_SIZE, alpha_size);
//                case EGL_NATIVE_BUFFER_USAGE_ANDROID:
//                    if (value & EGL_NATIVE_BUFFER_USAGE_PROTECTED_BIT_ANDROID) {
//                        usage |= USAGE_PROTECTED;
//                    }
//                    if (value & EGL_NATIVE_BUFFER_USAGE_RENDERBUFFER_ANDROID) {
//                        usage |= USAGE_HW_RENDER;
//                    }
//                    if (value & EGL_NATIVE_BUFFER_USAGE_TEXTURE_ANDROID) {
//                        usage |= USAGE_HW_TEXTURE;
//                    }
//                    // The buffer must be used for either a texture or a
//                    // renderbuffer.
//                    if ((value & EGL_NATIVE_BUFFER_USAGE_RENDERBUFFER_ANDROID) &&
//                        (value & EGL_NATIVE_BUFFER_USAGE_TEXTURE_ANDROID))
//                    {
////                        return setError(EGL_BAD_PARAMETER, (EGLClientBuffer)0);
//                        return NULL;
//                    }
//                    break;
                default:
//                    return setError(EGL_BAD_PARAMETER, (EGLClientBuffer)0);
                    return NULL;
            }
        }
    }
#undef GET_NONNEGATIVE_VALUE

    // Validate format.
    if (red_size == 8 && green_size == 8 && blue_size == 8) {
        if (alpha_size == 8) {
            format = PIXEL_FORMAT_RGBA_8888;
        } else {
            format = PIXEL_FORMAT_RGB_888;
        }
    } else if (red_size == 5 && green_size == 6 && blue_size == 5 &&
               alpha_size == 0) {
        format = PIXEL_FORMAT_RGB_565;
    } else {
//        ALOGE("Invalid native pixel format { r=%d, g=%d, b=%d, a=%d }", red_size, green_size, blue_size, alpha_size);
//        return setError(EGL_BAD_PARAMETER, (EGLClientBuffer)0);
        return NULL;
    }

#define CHECK_ERROR_CONDITION(message) \
    if (err != NO_ERROR) { \
        ALOGE(message); \
        goto error_condition; \
    }

//    // The holder is used to destroy the buffer if an error occurs.
//    GraphicBuffer* gBuffer = new GraphicBuffer();
//    sp<IServiceManager> sm = defaultServiceManager();
//    sp<IBinder> surfaceFlinger = sm->getService(String16("SurfaceFlinger"));
//    sp<IBinder> allocator;
//    Parcel sc_data, sc_reply, data, reply;
//    status_t err = NO_ERROR;
//    if (sm == NULL) {
//        ALOGE("Unable to connect to ServiceManager");
//        goto error_condition;
//    }
//
//    // Obtain an allocator.
//    if (surfaceFlinger == NULL) {
//        ALOGE("Unable to connect to SurfaceFlinger");
//        goto error_condition;
//    }
//    sc_data.writeInterfaceToken(String16("android.ui.ISurfaceComposer"));
//    err = surfaceFlinger->transact(
//            BnSurfaceComposer::CREATE_GRAPHIC_BUFFER_ALLOC, sc_data, &sc_reply);
//    CHECK_ERROR_CONDITION("Unable to obtain allocator from SurfaceFlinger");
//    allocator = sc_reply.readStrongBinder();
//
//    if (allocator == NULL) {
//        ALOGE("Unable to obtain an ISurfaceComposer");
//        goto error_condition;
//    }
//    data.writeInterfaceToken(String16("android.ui.IGraphicBufferAlloc"));
//    err = data.writeUint32(width);
//    CHECK_ERROR_CONDITION("Unable to write width");
//    err = data.writeUint32(height);
//    CHECK_ERROR_CONDITION("Unable to write height");
//    err = data.writeInt32(static_cast<int32_t>(format));
//    CHECK_ERROR_CONDITION("Unable to write format");
//    err = data.writeUint32(usage);
//    CHECK_ERROR_CONDITION("Unable to write usage");
//    err = data.writeUtf8AsUtf16(
//            std::string("[eglCreateNativeClientBufferANDROID pid ") +
//            std::to_string(getpid()) + ']');
//    CHECK_ERROR_CONDITION("Unable to write requestor name");
//    err = allocator->transact(IBinder::FIRST_CALL_TRANSACTION, data,
//                              &reply);
//    CHECK_ERROR_CONDITION(
//            "Unable to request buffer allocation from surface composer");
//    err = reply.readInt32();
//    CHECK_ERROR_CONDITION("Unable to obtain buffer from surface composer");
//    err = reply.read(*gBuffer);
//    CHECK_ERROR_CONDITION("Unable to read buffer from surface composer");
//
//    err = gBuffer->initCheck();
//    if (err != NO_ERROR) {
//        ALOGE("Unable to create native buffer { w=%d, h=%d, f=%d, u=%#x }: %#x",
//              width, height, format, usage, err);
//        goto error_condition;
//    }
//    ALOGD("Created new native buffer %p { w=%d, h=%d, f=%d, u=%#x }",
//          gBuffer, width, height, format, usage);
//    return static_cast<EGLClientBuffer>(gBuffer->getNativeBuffer());
//
//#undef CHECK_ERROR_CONDITION
//
//    error_condition:
//    // Delete the buffer.
//    sp<GraphicBuffer> holder(gBuffer);
//    return setError(EGL_BAD_ALLOC, (EGLClientBuffer)0);
}

///
// Create a simple 2x2 texture image with four different colors
//
GLuint CreateSimpleTexture2D( ESContext *esContext)
{
   // Texture object handle
   GLuint textureId;

   // 2x2 Image, 3 bytes per pixel (R, G, B)
   GLubyte pixels[4 * 4] =
   {
      255,   0,   0, 255, // Red
        0, 255,   0, 255, // Green
        0,   0, 255, 255, // Blue
      255, 255,   0, 255  // Yellow
   };

    ANativeWindowBuffer *pbuffer;

    int usage = USAGE_HW_TEXTURE | USAGE_SW_READ_OFTEN  | USAGE_SW_WRITE_RARELY;
    GraphicBuffer *graphicBuffer = new GraphicBuffer( 2, 2, PIXEL_FORMAT_RGBA_8888, usage, "string");
    graphicBuffer->initCheck();
    EGLClientBuffer clientBuffer = graphicBuffer->getNativeBuffer();

//    const EGLint buffer_att[] = {
//            EGL_WIDTH, 2,
//            EGL_HEIGHT, 2,
//            EGL_RED_SIZE, 8,
//            EGL_GREEN_SIZE, 8,
//            EGL_BLUE_SIZE, 8,
//            EGL_ALPHA_SIZE, 8,
//            EGL_NATIVE_BUFFER_USAGE_ANDROID, EGL_NATIVE_BUFFER_USAGE_TEXTURE_ANDROID,
//            EGL_NONE
//    };
//    EGLClientBuffer buffer = eglCreateNativeClientBufferANDROID(buffer_att);
    EGLint attrs[] = {
            EGL_IMAGE_PRESERVED_KHR,        EGL_TRUE,
            EGL_IMAGE_CROP_LEFT_ANDROID,    0,
            EGL_IMAGE_CROP_TOP_ANDROID,     0,
            EGL_IMAGE_CROP_RIGHT_ANDROID,   2,
            EGL_IMAGE_CROP_BOTTOM_ANDROID,  2,
            EGL_NONE,
    };

//    status_t err = graphicBuffer->initCheck();
//    if (err != OK)
//    {
//        LOGI("Error: %sn", strerror(-err));
//        return 0;
//    }
//
//    memcpy(buffer, pixels, 4*4);
    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
    // Generate a texture object
    glGenTextures ( 1, &textureId );
    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    EGLint eglImgAttrs[] = { EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE, EGL_NONE };
    esContext->pEGLImage = eglCreateImageKHR(eglGetCurrentDisplay(), NULL,
                                             EGL_NATIVE_BUFFER_ANDROID,
//                                             EGL_GL_TEXTURE_2D_KHR,
                                             (EGLClientBuffer)clientBuffer, eglImgAttrs);


//    esContext->pEGLImage = eglCreateImageKHR(eglGetCurrentDisplay(),
//                                             NULL,
//                                             EGL_NATIVE_BUFFER_ANDROID,
//                                             clientBuffer,
//                                             NULL);

//    EGLint eglImgAttrs[] = { EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE, EGL_NONE };
//    esContext->pEGLImage = eglCreateImageKHR(eglGetCurrentDisplay(),
//                                             eglGetCurrentContext(),
//                                             EGL_GL_TEXTURE_2D_KHR,
//                                             (EGLClientBuffer)textureId,
//                                             eglImgAttrs);


    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, esContext->pEGLImage);

    // Load the texture
//   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
    // Set the filtering mode


//    EGLint lockAttribList[] = {
//            EGL_LOCK_USAGE_HINT_KHR,
//            EGL_WRITE_SURFACE_BIT_KHR,
//            EGL_NONE
//    };
//    EGLBoolean lockResult = eglLockSurfaceKHR(esContext->eglDisplay, esContext->pEGLImage, lockAttribList);
//    EGLBoolean unlockResult = eglUnlockSurfaceKHR(esContext->eglDisplay, esContext->pEGLImage);

//    GLubyte *pdata;
//    graphicBuffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN, (void**)&pdata);
////    memset(pdata,128, 2*2);
//    memcpy(pdata, pixels, 3);
//    graphicBuffer->unlock();

   return textureId;

}


///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   char vShaderStr[] =
      "#version 300 es                            \n"
      "layout(location = 0) in vec4 a_position;   \n"
      "layout(location = 1) in vec2 a_texCoord;   \n"
      "out vec2 v_texCoord;                       \n"
      "void main()                                \n"
      "{                                          \n"
      "   gl_Position = a_position;               \n"
      "   v_texCoord = a_texCoord;                \n"
      "   gl_PointSize = 100.0;                   \n"
      "}                                          \n";

   char fShaderStr[] =
      "#version 300 es                                     \n"
      "precision mediump float;                            \n"
      "in vec2 v_texCoord;                                 \n"
      "layout(location = 0) out vec4 outColor;             \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  outColor = texture( s_texture, gl_PointCoord );   \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the sampler location
   userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

   // Load the texture
   userData->textureId = CreateSimpleTexture2D (esContext);

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                           -0.5f, -0.5f, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            0.5f, -0.5f, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                            0.5f,  0.5f, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );

   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->textureId );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
//    glDrawArrays ( GL_POINTS, 0, 6 );
}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->textureId );

   // Delete program object
   glDeleteProgram ( userData->programObject );

    eglDestroyImageKHR(esContext->eglDisplay,esContext->pEGLImage);
}


int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Simple Texture 2D", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterDrawFunc ( esContext, Draw );
   esRegisterShutdownFunc ( esContext, ShutDown );

   return GL_TRUE;
}
