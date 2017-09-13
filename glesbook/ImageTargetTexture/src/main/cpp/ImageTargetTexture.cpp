
#include <stdlib.h>
#include "esUtil.h"
#include "GraphicBuffer.h"
#include <EGL/eglext.h>
#include <GLES2/gl2ext.h>

GLuint androidTextureId;
GLuint framebufferId;

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Handle to a framebuffer object
   GLuint fbo;

   // Texture handle
   GLuint colorTexId;

    EGLImageKHR eglImage;

   // Texture size
   GLsizei textureWidth;
   GLsizei textureHeight;

} UserData;

typedef enum
{
	FBDEV_PIXMAP_DEFAULT = 0,
	FBDEV_PIXMAP_SUPPORTS_UMP = (1<<0),
	FBDEV_PIXMAP_ALPHA_FORMAT_PRE = (1<<1),
	FBDEV_PIXMAP_COLORSPACE_sRGB = (1<<2),
	FBDEV_PIXMAP_EGL_MEMORY = (1<<3),
	FBDEV_PIXMAP_DMA_BUF = (1<<4),
	FBDEV_PIXMAP_PHYADDR = (1<<5),
} fbdev_pixmap_flags;

typedef struct fbdev_pixmap
{
	unsigned int height;
	unsigned int width;
	unsigned int bytes_per_pixel;
	unsigned char buffer_size;
	unsigned char red_size;
	unsigned char green_size;
	unsigned char blue_size;
	unsigned char alpha_size;
	unsigned char luminance_size;
	fbdev_pixmap_flags flags;
	unsigned short *data;
	unsigned int format;
} fbdev_pixmap;

///
// Initialize the framebuffer object and MRTs
//
int InitFBO ( ESContext *esContext )
{
   UserData *userData = (UserData*)esContext->userData;
   GLint defaultFramebuffer = 0;
   const GLenum attachments[1] =
   { 
      GL_COLOR_ATTACHMENT0,
   };

   glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &defaultFramebuffer );

   // Setup fbo
   glGenFramebuffers ( 1, &userData->fbo );
   glBindFramebuffer ( GL_FRAMEBUFFER, userData->fbo );

   // Setup four output buffers and attach to fbo
   userData->textureHeight = userData->textureWidth = 400;
   glGenTextures ( 1, &userData->colorTexId );

   glBindTexture ( GL_TEXTURE_2D, userData->colorTexId);
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, userData->textureWidth, userData->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glFramebufferTexture2D ( GL_DRAW_FRAMEBUFFER, attachments[0], GL_TEXTURE_2D, userData->colorTexId, 0 );

    EGLint egl_img_attr[] = {
            EGL_GL_TEXTURE_LEVEL_KHR, 0,
            EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
    };

//    userData->eglImg = eglCreateImageKHR(esContext->eglDisplay, esContext->eglContext,EGL_GL_TEXTURE_2D_KHR,
//                                         (EGLClientBuffer)userData->colorTexId, egl_img_attr);
//
//    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)egl_img_attr);
//    GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//    if( framebufferStatus != GL_FRAMEBUFFER_COMPLETE){
//        return FALSE;
//    }


   glDrawBuffers ( 1, attachments );

   if ( GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus ( GL_FRAMEBUFFER ) )
   {
      return FALSE;
   }

   // Restore the original framebuffer
   glBindFramebuffer ( GL_FRAMEBUFFER, defaultFramebuffer );

   return TRUE;
}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   char vShaderStr[] =
      "attribute vec4 aPosition;    \n"
      "attribute vec4 aColor;       \n"
      "varying vec4 vColor;         \n"
      "void main()                  \n"
      "{                            \n"
      "    vColor = aColor;         \n"
      "    gl_Position = aPosition; \n"
      "}                            \n";

   char fShaderStr[] =
#ifndef NOEGLIMAGE
       "#extension GL_OES_EGL_image_external : require\n"
#endif
       "precision mediump float;    \n"
       "varying vec4 vColor;        \n"
#ifdef NOEGLIMAGE
       "uniform sampler2D uTexture;  \n"
#else
       "uniform samplerExternalOES uTexture;  \n"
#endif
       "uniform float uWidth;        \n"
       "uniform float uHeight;      \n"
       "                            \n"
       "void main()                  \n"
       "{                            \n"
       "    vec2 coord = gl_FragCoord.xy;\n"
       "    coord.x /= uWidth;\n"
       "    coord.y /= uHeight;\n"
       "    gl_FragColor = texture2D(uTexture, coord);  \n"
       "}                            \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

//   InitFBO ( esContext );

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}


///
// Copy MRT output buffers to screen
//
void BlitTextures ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   // set the fbo for reading
   glBindFramebuffer ( GL_READ_FRAMEBUFFER, userData->fbo );
 
   // Copy the output red buffer to lower left quadrant
   glReadBuffer ( GL_COLOR_ATTACHMENT0 );
   glBlitFramebuffer ( 0, 0, userData->textureWidth, userData->textureHeight,
                       0, 0, esContext->width, esContext->height,
                       GL_COLOR_BUFFER_BIT, GL_LINEAR );

}

void Draw( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
    PixelFormat format = PIXEL_FORMAT_RGBA_8888;
    int usage = GraphicBuffer::USAGE_HW_TEXTURE | GraphicBuffer::USAGE_SW_READ_OFTEN | GraphicBuffer::USAGE_SW_WRITE_RARELY;
    EGLClientBuffer clientBuffer = nullptr;
    GraphicBuffer *buffer = new GraphicBuffer(esContext->width, esContext->height, format, usage);
    clientBuffer = (EGLClientBuffer)buffer->getNativeBuffer();

    EGLint eglImgAttrs[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE, EGL_NONE};
    EGLImageKHR img = eglCreateImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_CONTEXT,
                            EGL_NATIVE_BUFFER_ANDROID,
                            (EGLClientBuffer) clientBuffer,
                            eglImgAttrs);

    glGenTextures(1, &androidTextureId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, androidTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, esContext->width, esContext->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Attach the EGLImage to whatever texture is bound to GL_TEXTURE_2D
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, img);
    glBindTexture(GL_TEXTURE_2D, 0);

   glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
   glViewport(0, 0, esContext->width, esContext->height);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, androidTextureId, 0);
   //render here, I've skipped it
   static GLfloat const aPositions [] = {
           -0.8f,
           -0.8f,
           0.0f,
           1.0f, 0.0f,
           0.8f,
           0.0f, 1.0f, 0.8f,
           -0.8f,
           0.0f,
           1.0f
   };
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, aPositions);
   glEnableVertexAttribArray(0);
   static GLfloat const aColors [] = {
           1.0f,
           0.0f,
           0.0f,
           1.0f,
           0.0f,
           1.0f,
           0.0f,
           1.0f,
           0.0f,
           0.0f,
           1.0f,
           1.0f
   };
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, aColors);
   glEnableVertexAttribArray(1);
   glUniform1i(glGetUniformLocation(userData->programObject, "uTexture"), 0);
   glUniform1f(glGetUniformLocation(userData->programObject, "uWidth"), esContext->width);
   glUniform1f(glGetUniformLocation(userData->programObject, "uHeight"), esContext->height);

   static uint8_t textureData[4 * 4][4] = {
           {0x00, 0x00, 0x00, 0xff},
           {0x00, 0x00, 0x7f, 0xff},
           {0x00, 0x7f, 0x00, 0xff},
           {0x00, 0x7f, 0x7f, 0xff},


           {0x00, 0xff, 0x00, 0xff},
           {0x00, 0xff, 0x7f, 0xff},
           {0x7f, 0x00, 0x00, 0xff},
           {0x7f, 0x00, 0x7f, 0xff},


           {0x7f, 0x7f, 0x00, 0xff},
           {0x7f, 0x7f, 0x7f, 0xff},
           {0x7f, 0xff, 0x00, 0xff},
           {0x7f, 0xff, 0x7f, 0xff},


           {0xff, 0x00, 0x00, 0xff},
           {0xff, 0x00, 0x7f, 0xff},
           {0xff, 0x7f, 0x00, 0xff},
           {0xff, 0x7f, 0x7f, 0xff},
   };



   glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

//   glFinish();
//   void *memPtr;
//   buffer->lock(mode, &memPtr);
//   glActiveTexture(GL_TEXTURE0);
//   glPixelStorei(GL_PACK_ALIGNMENT, 1);
//   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//   glBindTexture(GL_TEXTURE_2D, textureToRenderId);
//   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, buffer->stride, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, memPtr);
//   unlockGraphicsBuffer();

   //render here, I've skipped it
   glBindTexture(GL_TEXTURE_2D, 0);
   glUseProgram(0);
}

void Draw_1( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   glUseProgram ( userData->programObject );
   glClearColor(0.2, 0.2, 0.2, 1.0);
   static GLfloat const aPositions [] = {
           -0.8f,
           -0.8f,
           0.0f,
           1.0f, 0.0f,
           0.8f,
           0.0f, 1.0f, 0.8f,
           -0.8f,
           0.0f,
           1.0f
   };
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, aPositions);
   glEnableVertexAttribArray(0);
   static GLfloat const aColors [] = {
           1.0f,
           0.0f,
           0.0f,
           1.0f,
           0.0f,
           1.0f,
           0.0f,
           1.0f,
           0.0f,
           0.0f,
           1.0f,
           1.0f
   };
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, aColors);
   glEnableVertexAttribArray(1);

   GLuint texture = 0;
   glGenTextures(1, &texture);
   glActiveTexture(GL_TEXTURE0);


   glUniform1i(glGetUniformLocation(userData->programObject, "uTexture"), 0);
   glUniform1f(glGetUniformLocation(userData->programObject, "uWidth"), esContext->width);
   glUniform1f(glGetUniformLocation(userData->programObject, "uHeight"), esContext->height);

   static uint8_t textureData[4 * 4][4] = {
           {0x00, 0x00, 0x00, 0xff},
           {0x00, 0x00, 0x7f, 0xff},
           {0x00, 0x7f, 0x00, 0xff},
           {0x00, 0x7f, 0x7f, 0xff},


           {0x00, 0xff, 0x00, 0xff},
           {0x00, 0xff, 0x7f, 0xff},
           {0x7f, 0x00, 0x00, 0xff},
           {0x7f, 0x00, 0x7f, 0xff},


           {0x7f, 0x7f, 0x00, 0xff},
           {0x7f, 0x7f, 0x7f, 0xff},
           {0x7f, 0xff, 0x00, 0xff},
           {0x7f, 0xff, 0x7f, 0xff},


           {0xff, 0x00, 0x00, 0xff},
           {0xff, 0x00, 0x7f, 0xff},
           {0xff, 0x7f, 0x00, 0xff},
           {0xff, 0x7f, 0x7f, 0xff},
   };


   fbdev_pixmap pixmap = {};


   pixmap.height = 4;
   pixmap.width = 4;
   pixmap.bytes_per_pixel = 4;
   pixmap.buffer_size = 32;
   pixmap.red_size = 8;
   pixmap.green_size = 8;
   pixmap.blue_size = 8;
   pixmap.alpha_size = 8;
   pixmap.luminance_size = 0;
   pixmap.flags = static_cast<fbdev_pixmap_flags>(FBDEV_PIXMAP_DEFAULT);
   pixmap.data = reinterpret_cast<unsigned short*>(textureData);
   pixmap.format = 0;


//   EGLint const imageAttributes [] = {
//           EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE, EGL_NONE
//   };
//
//   userData->eglImage = eglCreateImageKHR(eglGetCurrentDisplay(),
//                                  eglGetCurrentContext(),
//                                          EGL_NATIVE_PIXMAP_KHR,
//                                  reinterpret_cast<EGLClientBuffer>(textureData),
//                                  imageAttributes);

   EGLint eglImgAttrs[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE, EGL_NONE};
   EGLImageKHR img = eglCreateImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_CONTEXT,
                                       EGL_NATIVE_BUFFER_ANDROID,
                                       (EGLClientBuffer) (&pixmap),
                                       eglImgAttrs);


   glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
   glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, userData->eglImage);
   glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   // Doesn't work
   /*
   EGLint bitmapAdress, bitmapPitch;
   eglQuerySurface(display, image, EGL_BITMAP_POINTER_KHR, &bitmapAdress);
   assert(eglGetError() == EGL_SUCCESS);
   eglQuerySurface(display, image, EGL_BITMAP_PITCH_KHR, &bitmapPitch);
   assert(eglGetError() == EGL_SUCCESS);
   printf("Address: %x\nPitch %d\n", bitmapAdress, bitmapPitch);
   */

   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   glDrawArrays(GL_TRIANGLES, 0, 3);

   EGLint lockAttribList[] = {
           EGL_LOCK_USAGE_HINT_KHR,
           EGL_WRITE_SURFACE_BIT_KHR,
           EGL_NONE
   };


   // Doesn't work
   EGLBoolean lockResult = eglLockSurfaceKHR(esContext->eglDisplay, userData->eglImage, lockAttribList);

   // update the texture data
   for (auto i = 0; i < 4; ++i) {
      for (auto x = 0; x < 4; ++x) {
         for (auto y = 0; y < 4; ++y) {
            textureData[4 * x + y][i]++;
         }
      }
   }
   EGLBoolean unlockResult = eglUnlockSurfaceKHR(esContext->eglDisplay, userData->eglImage);

   glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
   glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
   //glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, 0);
   glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, userData->eglImage);
    return;
}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   // Delete texture objects
   glDeleteTextures ( 4, &userData->colorTexId );

   // Delete fbo
   glDeleteFramebuffers ( 1, &userData->fbo );

   // Delete program object
   glDeleteProgram ( userData->programObject );

   eglDestroyImageKHR(esContext->eglDisplay, userData->eglImage);

}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Multiple Render Targets", 400, 400, ES_WINDOW_RGB | ES_WINDOW_ALPHA );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterDrawFunc ( esContext, Draw );
   esRegisterShutdownFunc ( esContext, ShutDown );

   return GL_TRUE;
}
