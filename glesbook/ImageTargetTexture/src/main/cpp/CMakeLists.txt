cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fPIC  -shared -fexceptions")

ADD_DEFINITIONS(-DEGL_EGLEXT_PROTOTYPES)
ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)

add_library(glesbook SHARED
            android_native_app_glue.c
            esShader.cpp
            esShapes.cpp
            esTransform.cpp
            esUtil.cpp
            esUtil_Android.cpp
            GraphicBuffer.cpp
            DynamicLibrary.cpp
            ImageTargetTexture.cpp)

# Include libraries needed for hello-jni lib
target_link_libraries(glesbook
                      android
                      log
                      EGL
                      GLESv3)
