cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++11 -fno-exceptions -Wall -fno-rtti -Wno-error=format-security \
                     ")

add_library(gles3jni SHARED
           gles3jni.cpp
           Renderer.cpp
           RendererES2.cpp
           RendererES3.cpp
           ShareContextRender.cpp
           ThreadPool.cpp
           gl3stub_wrapper.c
            )

target_link_libraries(gles3jni
                      android
                      log
                      GLESv3
                      EGL )



