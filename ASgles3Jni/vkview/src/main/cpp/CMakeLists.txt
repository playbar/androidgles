cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++11 -fno-exceptions -Wall -fno-rtti -Wno-error=format-security \
                     ")

add_library(vkview SHARED
           gles3jni.cpp
           RendererES2.cpp
           RendererES3.cpp
           ThreadPool.cpp
           ImageSobelFilter.cpp
           gl3stub_wrapper.c
           aopencl.c
           hook/inlineHook.c
           hook/relocate.c
           hook/gleshook.c
            )

target_link_libraries(vkview
                      android
                      log
                      GLESv3
                      EGL )



