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
        zlib/adler32.c
        zlib/compress.c
        zlib/crc32.c
        zlib/deflate.c
        zlib/gzclose.c
        zlib/gzlib.c
        zlib/gzread.c
        zlib/gzwrite.c
        zlib/infback.c
        zlib/inffast.c
        zlib/inflate.c
        zlib/inftrees.c
        zlib/trees.c
        zlib/uncompr.c
        zlib/zutil.c
        android_native_app_glue.c
        esShader.c
        esShapes.c
        esTransform.c
        esUtil.c
        esUtil_Android.c
        Texture2D.c
        inlineHook.c
        relocate.c
        gleshook.c
        gldrawback.c
        AssetStorage.cpp
        EGLConfigChooser.cpp
        nativeInterface.cpp
        render.cpp)

target_include_directories(glesbook PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/libpng )

# Include libraries needed for hello-jni lib
target_link_libraries(glesbook
                      android
                      z
                      log
                      jnigraphics
                      EGL
                      GLESv3)
