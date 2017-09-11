
#include <stdlib.h>
#include "esUtil.h"
#include <EGL/eglext.h>
#include <GLES2/gl2ext.h>

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Handle to a framebuffer object
   GLuint fbo;

   // Texture handle
   GLuint colorTexId;

    EGLImageKHR eglImg;

   // Texture size
   GLsizei textureWidth;
   GLsizei textureHeight;

} UserData;

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

    userData->eglImg = eglCreateImageKHR(esContext->eglDisplay, esContext->eglContext,EGL_GL_TEXTURE_2D_KHR,
                                         (EGLClientBuffer)userData->colorTexId, egl_img_attr);

    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)egl_img_attr);
    GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( framebufferStatus != GL_FRAMEBUFFER_COMPLETE){
        return FALSE;
    }


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
      "#version 300 es                            \n"
      "layout(location = 0) in vec4 a_position;   \n"
      "void main()                                \n"
      "{                                          \n"
      "   gl_Position = a_position;               \n"
      "}                                          \n";

   char fShaderStr[] =
       "#version 300 es                                     \n"
       "#extension GL_OES_EGL_image_external : require      \n"
       "precision mediump float;                            \n"
       "layout(location = 0) out vec4 fragData0;            \n"
       "void main()                                         \n"
       "{                                                   \n"
       "  // first buffer will contain red color            \n"
       "  fragData0 = vec4 ( 1, 0, 0, 1 );                  \n"
       "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   InitFBO ( esContext );

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a quad and output four colors per pixel
//
void DrawGeometry ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,
                           -1.0f, -1.0f, 0.0f,
                            1.0f, -1.0f, 0.0f,
                            1.0f,  1.0f, 0.0f,
                         };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof ( GLfloat ), vVertices );
   glEnableVertexAttribArray ( 0 );

   // Draw a quad
   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
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

///
// Render to MRTs and screen
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   GLint defaultFramebuffer = 0;
   const GLenum attachments[1] =
   { 
      GL_COLOR_ATTACHMENT0,
   };
   
   glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &defaultFramebuffer );

   // FIRST: use MRTs to output four colors to four buffers
   glBindFramebuffer ( GL_FRAMEBUFFER, userData->fbo );
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   glDrawBuffers ( 1, attachments );
   DrawGeometry ( esContext );

   // SECOND: copy the four output buffers into four window quadrants
   // with framebuffer blits

   // Restore the default framebuffer
   glBindFramebuffer ( GL_DRAW_FRAMEBUFFER, defaultFramebuffer );
   BlitTextures ( esContext );
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
