cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fPIC  -shared -Wno-error, -Wno-format-security -fexceptions")

ADD_DEFINITIONS(-DEGL_EGLEXT_PROTOTYPES)
ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)

add_library(pbuffer SHARED
        glbackdraw.cpp
        GraphicBuffer.cpp
        DynamicLibrary.cpp)

# Include libraries needed for hello-jni lib
target_link_libraries(pbuffer
                      android
                      z
                      log
                      jnigraphics
                      EGL
                      GLESv2)
