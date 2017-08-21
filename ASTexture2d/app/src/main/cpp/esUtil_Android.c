#include <android/log.h>
#include "android_native_app_glue.h"
#include <time.h>
#include "esUtil.h"
#include "gleshook.h"

///
//  Global extern.  The application must declare this function
//  that runs the application.
//
extern int esMain ( ESContext *esContext );

//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//

///
// GetCurrentTime()
//
static float GetCurrentTime()
{
   struct timespec clockRealTime;
   clock_gettime ( CLOCK_MONOTONIC, &clockRealTime );
   double curTimeInSeconds = clockRealTime.tv_sec + ( double ) clockRealTime.tv_nsec / 1e9;
   return ( float ) curTimeInSeconds;
}

///
// HandleCommand()
//
//    Android callback for onAppCmd
//
static void HandleCommand ( struct android_app *pApp, int32_t cmd )
{
   ESContext *esContext = ( ESContext * ) pApp->userData;

   switch ( cmd )
   {
      case APP_CMD_SAVE_STATE:
         // the OS asked us to save the state of the app
         break;

      case APP_CMD_INIT_WINDOW:

         esContext->eglNativeDisplay = EGL_DEFAULT_DISPLAY;
         esContext->eglNativeWindow = pApp->window;

         // Call the main entrypoint for the app
         if ( esMain ( esContext ) != GL_TRUE )
         {
            exit ( 0 ); //@TEMP better way to exit?
         }

         break;

      case APP_CMD_TERM_WINDOW:

         // Cleanup on shutdown
         if ( esContext->shutdownFunc != NULL )
         {
            esContext->shutdownFunc ( esContext );
         }

         if ( esContext->userData != NULL )
         {
            free ( esContext->userData );
         }

         memset ( esContext, 0, sizeof ( ESContext ) );
         break;

      case APP_CMD_LOST_FOCUS:
         // if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
         break;

      case APP_CMD_GAINED_FOCUS:
         // bring back a certain functionality, like monitoring the accelerometer
         break;
   }
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  android_main()
//
//    Main entrypoint for Android application
//
void android_main ( struct android_app *pApp )
{
   ESContext esContext;
   float lastTime;

   // Make sure glue isn't stripped.
   app_dummy();
    hookAllFun();

   // Initialize the context
   memset ( &esContext, 0, sizeof ( ESContext ) );

   esContext.platformData = ( void * ) pApp->activity->assetManager;

   pApp->onAppCmd = HandleCommand;
   pApp->userData = &esContext;

   lastTime = GetCurrentTime();

   while ( 1 )
   {
      int ident;
      int events;
      struct android_poll_source *pSource;

      while ( ( ident = ALooper_pollAll ( 0, NULL, &events, ( void ** ) &pSource ) ) >= 0 )
      {

         if ( pSource != NULL )
         {
            pSource->process ( pApp, pSource );
         }

         if ( pApp->destroyRequested != 0 )
         {
            return;
         }

      }

      if ( esContext.eglNativeWindow == NULL )
      {
         continue;
      }

      // Call app update function
      if ( esContext.updateFunc != NULL )
      {
         float curTime = GetCurrentTime();
         float deltaTime =  ( curTime - lastTime );
         lastTime = curTime;
         esContext.updateFunc ( &esContext, deltaTime );
      }

      if ( esContext.drawFunc != NULL )
      {
         esContext.drawFunc ( &esContext );
         eglSwapBuffers ( esContext.eglDisplay, esContext.eglWSurface );
//          eglSwapBuffers ( esContext.eglDisplay, esContext.eglPSurface );
      }
   }
}


///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate ( ESContext *esContext, const char *title )
{
   // On Android, this does not need to do anything.  Everything happens from
   // android_main()
   return GL_TRUE;
}
