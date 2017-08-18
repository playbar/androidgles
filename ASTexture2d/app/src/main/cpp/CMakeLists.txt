cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fPIC  -shared -Wno-error, -Wno-format-security")

add_library(glesbook SHARED
        libpng/png.c
        libpng/pngerror.c
        libpng/pngget.c
        libpng/pngmem.c
        libpng/pngpread.c
        libpng/pngread.c
        libpng/pngrio.c
        libpng/pngrtran.c
        libpng/pngrutil.c
        libpng/pngset.c
        libpng/pngtest.c
        libpng/pngtrans.c
        libpng/pngwio.c
        libpng/pngwrite.c
        libpng/pngwtran.c
        libpng/pngwutil.c
        libpng/pngutils.c
        android_native_app_glue.c
        esShader.c
        esShapes.c
        esTransform.c
        esUtil.c
        esUtil_Android.c
        Texture2D.c
        inlineHook.c
        relocate.c
        gleshook.c)

# Include libraries needed for hello-jni lib
target_link_libraries(glesbook
                      android
                      z
                      log
                      EGL
                      GLESv3)
