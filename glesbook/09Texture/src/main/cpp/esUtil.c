// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// ESUtil.c
//
//    A utility library for OpenGL ES.  This library provides a
//    basic common framework for the example applications in the
//    OpenGL ES 3.0 Programming Guide.
//

///
//  Includes
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "esUtil.h"
#include "esUtil_win.h"

#ifdef ANDROID
#include <android/log.h>
#include "android_native_app_glue.h"
#include <android/asset_manager.h>
typedef AAsset esFile;
#else
typedef FILE esFile;
#endif

#ifdef __APPLE__
#include "FileWrapper.h"
#endif

///
//  Macros
//
#define INVERTED_BIT            (1 << 5)

///
//  Types
//
#ifndef __APPLE__
#pragma pack(push,x1)                            // Byte alignment (8-bit)
#pragma pack(1)
#endif

typedef struct
#ifdef __APPLE__
__attribute__ ( ( packed ) )
#endif
{
   unsigned char  IdSize,
            MapType,
            ImageType;
   unsigned short PaletteStart,
            PaletteSize;
   unsigned char  PaletteEntryDepth;
   unsigned short X,
            Y,
            Width,
            Height;
   unsigned char  ColorDepth,
            Descriptor;

} TGA_HEADER;

#ifndef __APPLE__
#pragma pack(pop,x1)
#endif

#ifndef __APPLE__

///
// GetContextRenderableType()
//
//    Check whether EGL_KHR_create_context extension is supported.  If so,
//    return EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT
//
EGLint GetContextRenderableType ( EGLDisplay eglDisplay )
{
#ifdef EGL_KHR_create_context
   const char *extensions = eglQueryString ( eglDisplay, EGL_EXTENSIONS );

   // check whether EGL_KHR_create_context is in the extension string
   if ( extensions != NULL && strstr( extensions, "EGL_KHR_create_context" ) )
   {
      // extension is supported
      return EGL_OPENGL_ES3_BIT_KHR;
   }
#endif
   // extension is not supported
   return EGL_OPENGL_ES2_BIT;
}
#endif

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
//
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags )
{
#ifndef __APPLE__
   EGLConfig config;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };

   if ( esContext == NULL )
   {
      return GL_FALSE;
   }

#ifdef ANDROID
   // For Android, get the width/height from the window rather than what the
   // application requested.
   esContext->width = ANativeWindow_getWidth ( esContext->eglNativeWindow );
   esContext->height = ANativeWindow_getHeight ( esContext->eglNativeWindow );
#else
   esContext->width = width;
   esContext->height = height;
#endif

   if ( !WinCreate ( esContext, title ) )
   {
      return GL_FALSE;
   }

   esContext->eglDisplay = eglGetDisplay( esContext->eglNativeDisplay );
   if ( esContext->eglDisplay == EGL_NO_DISPLAY )
   {
      return GL_FALSE;
   }

   // Initialize EGL
   if ( !eglInitialize ( esContext->eglDisplay, &majorVersion, &minorVersion ) )
   {
      return GL_FALSE;
   }

   {
      EGLint numConfigs = 0;
      EGLint attribList[] =
      {
         EGL_RED_SIZE,       5,
         EGL_GREEN_SIZE,     6,
         EGL_BLUE_SIZE,      5,
         EGL_ALPHA_SIZE,     ( flags & ES_WINDOW_ALPHA ) ? 8 : EGL_DONT_CARE,
         EGL_DEPTH_SIZE,     ( flags & ES_WINDOW_DEPTH ) ? 8 : EGL_DONT_CARE,
         EGL_STENCIL_SIZE,   ( flags & ES_WINDOW_STENCIL ) ? 8 : EGL_DONT_CARE,
         EGL_SAMPLE_BUFFERS, ( flags & ES_WINDOW_MULTISAMPLE ) ? 1 : 0,
         // if EGL_KHR_create_context extension is supported, then we will use
         // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
         EGL_RENDERABLE_TYPE, GetContextRenderableType ( esContext->eglDisplay ),
         EGL_NONE
      };

      // Choose config
      if ( !eglChooseConfig ( esContext->eglDisplay, attribList, &config, 1, &numConfigs ) )
      {
         return GL_FALSE;
      }

      int _alpha_size = 0;
      int _bind_to_texture_rgb = 0;
      int _bind_to_texture_rgba = 0;
      int _blue_size = 0;
      int _buffer_size = 0;
      int _config_caveat = 0;
      int _config_id = 0;
      int _depth_size = 0;
      int _green_size = 0;
      int _red_size = 0;
      int _level = 0;
      int _stencil_size = 0;
      int _max_pbuffer_width = 0;
      int _max_pbuffer_height = 0;
      int _max_pbuffer_pixels = 0;
      int _max_swap_interval = 0;
      int _min_swap_interval = 0;
      int _native_renderable = 0;
      int _native_visula_id = 0;
      int _native_visual_type = 0;
      int _samples = 0;
      int _samples_buffers = 0;
      int _surface_type = 0;
      int _transparent_type = 0;
      int _transparent_blue_value = 0;
      int _transparent_green_value = 0;
      int _transparent_red_value = 0;
      int _alpha_mask_size = 0;
      int _color_buffer_type = 0;
      int _luminance_size = 0;
      int _renderable_type = 0;
      int _match_native_pixmap = 0;
      int _conformant = 0;
      EGLDisplay  _eglDisplay = esContext->eglDisplay;
      for ( GLint c = 0 ; c < numConfigs ; ++c)
      {
         eglGetConfigAttrib( _eglDisplay, config, EGL_BUFFER_SIZE, &(_buffer_size));
         eglGetConfigAttrib( _eglDisplay, config, EGL_ALPHA_SIZE, &_alpha_size);
         eglGetConfigAttrib( _eglDisplay, config, EGL_BLUE_SIZE, &(_blue_size));
         eglGetConfigAttrib( _eglDisplay, config, EGL_GREEN_SIZE, &(_green_size));
         eglGetConfigAttrib( _eglDisplay, config, EGL_RED_SIZE, &(_red_size));
         eglGetConfigAttrib( _eglDisplay, config, EGL_DEPTH_SIZE, &(_depth_size));
         eglGetConfigAttrib( _eglDisplay, config, EGL_STENCIL_SIZE, &(_stencil_size));
         eglGetConfigAttrib( _eglDisplay, config, EGL_CONFIG_CAVEAT, &(_config_caveat));
         eglGetConfigAttrib( _eglDisplay, config, EGL_CONFIG_ID, &(_config_id));
         eglGetConfigAttrib( _eglDisplay, config, EGL_LEVEL, &(_level));
         eglGetConfigAttrib( _eglDisplay, config, EGL_MAX_PBUFFER_HEIGHT, &(_max_pbuffer_height));
         eglGetConfigAttrib( _eglDisplay, config, EGL_MAX_PBUFFER_PIXELS, &(_max_pbuffer_pixels));
         eglGetConfigAttrib( _eglDisplay, config, EGL_MAX_PBUFFER_WIDTH, &(_max_pbuffer_width));
         eglGetConfigAttrib( _eglDisplay, config, EGL_NATIVE_RENDERABLE, &(_native_renderable));
         eglGetConfigAttrib( _eglDisplay, config, EGL_NATIVE_VISUAL_ID, &(_native_visula_id));
         eglGetConfigAttrib( _eglDisplay, config, EGL_NATIVE_VISUAL_TYPE, &(_native_visual_type));
         eglGetConfigAttrib( _eglDisplay, config, EGL_SAMPLES, &(_samples));
         eglGetConfigAttrib( _eglDisplay, config, EGL_SAMPLE_BUFFERS, &(_samples_buffers));
         eglGetConfigAttrib( _eglDisplay, config, EGL_SURFACE_TYPE, &(_surface_type));
         eglGetConfigAttrib( _eglDisplay, config, EGL_TRANSPARENT_TYPE, &(_transparent_type));
         eglGetConfigAttrib( _eglDisplay, config, EGL_TRANSPARENT_BLUE_VALUE, &(_transparent_blue_value));
         eglGetConfigAttrib( _eglDisplay, config, EGL_TRANSPARENT_GREEN_VALUE, &(_transparent_green_value));
         eglGetConfigAttrib( _eglDisplay, config, EGL_TRANSPARENT_RED_VALUE, &(_transparent_red_value));
         eglGetConfigAttrib( _eglDisplay, config, EGL_BIND_TO_TEXTURE_RGB, &(_bind_to_texture_rgb));
         eglGetConfigAttrib( _eglDisplay, config, EGL_BIND_TO_TEXTURE_RGBA, &(_bind_to_texture_rgba));
         eglGetConfigAttrib( _eglDisplay, config, EGL_MIN_SWAP_INTERVAL, &(_min_swap_interval));
         eglGetConfigAttrib( _eglDisplay, config, EGL_MAX_SWAP_INTERVAL, &(_max_swap_interval));



         /// etc etc etc for all those that you care about

         if ( majorVersion >= 1 && minorVersion >= 2 )
         {
            // 1.2
            eglGetConfigAttrib( _eglDisplay, config, EGL_LUMINANCE_SIZE, &(_luminance_size));
            eglGetConfigAttrib( _eglDisplay, config, EGL_ALPHA_MASK_SIZE, &(_alpha_mask_size));
            eglGetConfigAttrib( _eglDisplay, config, EGL_COLOR_BUFFER_TYPE, &(_color_buffer_type));
            eglGetConfigAttrib( _eglDisplay, config, EGL_RENDERABLE_TYPE, &(_renderable_type));
         }

         eglGetConfigAttrib( _eglDisplay, config, EGL_MATCH_NATIVE_PIXMAP, &(_match_native_pixmap));
         if ( majorVersion >= 1 && minorVersion >= 3 )
         {
            // 1.3
            eglGetConfigAttrib( _eglDisplay, config, EGL_CONFORMANT, &(_conformant));
         }

      }


      if ( numConfigs < 1 )
      {
         return GL_FALSE;
      }
   }


#ifdef ANDROID
   // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
   {
      EGLint format = 0;
      eglGetConfigAttrib ( esContext->eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format );
      ANativeWindow_setBuffersGeometry ( esContext->eglNativeWindow, 0, 0, format );
   }
#endif // ANDROID

   // Create a surface
   esContext->eglSurface = eglCreateWindowSurface ( esContext->eglDisplay, config, 
                                                    esContext->eglNativeWindow, NULL );

   if ( esContext->eglSurface == EGL_NO_SURFACE )
   {
      return GL_FALSE;
   }

   // Create a GL context
   esContext->eglContext = eglCreateContext ( esContext->eglDisplay, config, 
                                              EGL_NO_CONTEXT, contextAttribs );

   if ( esContext->eglContext == EGL_NO_CONTEXT )
   {
      return GL_FALSE;
   }

   // Make the context current
   if ( !eglMakeCurrent ( esContext->eglDisplay, esContext->eglSurface, 
                          esContext->eglSurface, esContext->eglContext ) )
   {
      return GL_FALSE;
   }

#endif // #ifndef __APPLE__

   return GL_TRUE;
}

///
//  esRegisterDrawFunc()
//
void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void ( ESCALLBACK *drawFunc ) ( ESContext * ) )
{
   esContext->drawFunc = drawFunc;
}

///
//  esRegisterShutdownFunc()
//
void ESUTIL_API esRegisterShutdownFunc ( ESContext *esContext, void ( ESCALLBACK *shutdownFunc ) ( ESContext * ) )
{
   esContext->shutdownFunc = shutdownFunc;
}

///
//  esRegisterUpdateFunc()
//
void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void ( ESCALLBACK *updateFunc ) ( ESContext *, float ) )
{
   esContext->updateFunc = updateFunc;
}


///
//  esRegisterKeyFunc()
//
void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,
                                    void ( ESCALLBACK *keyFunc ) ( ESContext *, unsigned char, int, int ) )
{
   esContext->keyFunc = keyFunc;
}


///
// esLogMessage()
//
//    Log an error message to the debug output for the platform
//
void ESUTIL_API esLogMessage ( const char *formatStr, ... )
{
   va_list params;
   char buf[BUFSIZ];

   va_start ( params, formatStr );
   vsprintf ( buf, formatStr, params );

#ifdef ANDROID
   __android_log_print ( ANDROID_LOG_INFO, "esUtil" , "%s", buf );
#else
   printf ( "%s", buf );
#endif

   va_end ( params );
}

///
// esFileRead()
//
//    Wrapper for platform specific File open
//
static esFile *esFileOpen ( void *ioContext, const char *fileName )
{
   esFile *pFile = NULL;

#ifdef ANDROID

   if ( ioContext != NULL )
   {
      AAssetManager *assetManager = ( AAssetManager * ) ioContext;
      pFile = AAssetManager_open ( assetManager, fileName, AASSET_MODE_BUFFER );
   }

#else
#ifdef __APPLE__
   // iOS: Remap the filename to a path that can be opened from the bundle.
   fileName = GetBundleFileName ( fileName );
#endif

   pFile = fopen ( fileName, "rb" );
#endif

   return pFile;
}

///
// esFileRead()
//
//    Wrapper for platform specific File close
//
static void esFileClose ( esFile *pFile )
{
   if ( pFile != NULL )
   {
#ifdef ANDROID
      AAsset_close ( pFile );
#else
      fclose ( pFile );
      pFile = NULL;
#endif
   }
}

///
// esFileRead()
//
//    Wrapper for platform specific File read
//
static int esFileRead ( esFile *pFile, int bytesToRead, void *buffer )
{
   int bytesRead = 0;

   if ( pFile == NULL )
   {
      return bytesRead;
   }

#ifdef ANDROID
   bytesRead = AAsset_read ( pFile, buffer, bytesToRead );
#else
   bytesRead = fread ( buffer, bytesToRead, 1, pFile );
#endif

   return bytesRead;
}

///
// esLoadTGA()
//
//    Loads a 8-bit, 24-bit or 32-bit TGA image from a file
//
char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height )
{
   char        *buffer;
   esFile      *fp;
   TGA_HEADER   Header;
   int          bytesRead;

   // Open the file for reading
   fp = esFileOpen ( ioContext, fileName );

   if ( fp == NULL )
   {
      // Log error as 'error in opening the input file from apk'
      esLogMessage ( "esLoadTGA FAILED to load : { %s }\n", fileName );
      return NULL;
   }

   bytesRead = esFileRead ( fp, sizeof ( TGA_HEADER ), &Header );

   *width = Header.Width;
   *height = Header.Height;

   if ( Header.ColorDepth == 8 ||
         Header.ColorDepth == 24 || Header.ColorDepth == 32 )
   {
      int bytesToRead = sizeof ( char ) * ( *width ) * ( *height ) * Header.ColorDepth / 8;

      // Allocate the image data buffer
      buffer = ( char * ) malloc ( bytesToRead );

      if ( buffer )
      {
         bytesRead = esFileRead ( fp, bytesToRead, buffer );
         esFileClose ( fp );

         return ( buffer );
      }
   }

   return ( NULL );
}
