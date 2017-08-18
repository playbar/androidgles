cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fPIC  -shared -Wno-error, -Wno-format-security")

add_library(glesbook SHARED
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
                      log
                      EGL
                      GLESv3)
