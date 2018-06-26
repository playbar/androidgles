cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#                    -std=c++11 -fno-exceptions -Wall -fno-rtti -Wno-error=format-security \")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -fPIC  -Wa,-mimplicit-it=thumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wa,-mimplicit-it=thumb ")

ADD_DEFINITIONS(-DEGL_EGLEXT_PROTOTYPES)
ADD_DEFINITIONS(-DGL_GLEXT_PROTOTYPES)

add_library(gles3jni SHARED
           gles3jni.cpp
           RendererES2.cpp
           RendererES3.cpp
           ThreadPool.cpp
           ImageSobelFilter.cpp
           gl3stub_wrapper.c
           aopencl.c
            )

target_link_libraries(gles3jni
                      android
                      log
                      GLESv3
                      EGL )

################

add_library(glhook SHARED
               gleshook/inlineHook.c
               gleshook/hookutils.c
               gleshook/relocate.c
               gleshook/eglexthook.c
               gleshook/gl2exthook.c
               gleshook/eglhook.c
               gleshook/gleshook.c
               gleshook/threadhook.c
 #              gleshook/vkhook.c
               gleshook/callstack.c
               gleshook/elf_common.cpp
               gleshook/elf_hooker.cpp
               gleshook/elf_main.cpp
               gleshook/elf_module.cpp
               gleshook/exporthook.c
               gleshook/exportutil.c
               gleshook/andhook.c
               #gleshook/syscallstack.c
               )

target_include_directories(glhook PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          )

target_link_libraries(glhook
                     log
                     android
 #                   lib_EH
 #                   vulkan
                     GLESv3
                     EGL)

