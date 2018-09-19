cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++11 -fexceptions -Wall -fno-rtti -Wno-error=format-security \
                    -DVK_USE_PLATFORM_ANDROID_KHR")

add_subdirectory(shaderc)

add_library(glview SHARED
           gl/gles3jni.cpp
           gl/RendererES2.cpp
           gl/RendererES3.cpp
           gl/ThreadPool.cpp
           gl/gl3stub_wrapper.c
           hook/inlineHook.c
           hook/relocate.c
           hook/gleshook.c
            )

target_include_directories(glview PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/gl
                                      )

target_link_libraries(glview
                      android
                      log
                      GLESv3
                      EGL )


######

add_library(vkutils SHARED
           vk/vulkan_utils.cpp
           vk/vulkan_buffer.cpp
           vk/vulkan_texture.cpp
           vk/vulkan_device.cpp
           vk/vulkan_wrapper.cpp
           vk/vulkan_utils_jni.cpp
            )

target_include_directories(vkutils PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/vk
                          ${CMAKE_CURRENT_SOURCE_DIR}/data
                          ${CMAKE_CURRENT_SOURCE_DIR}/stb
                          ${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader
                          ${CMAKE_CURRENT_SOURCE_DIR}/shaderc/libshaderc/include
                          )

target_link_libraries(vkutils
                      android
                      log
                      shaderc
                      shaderc_util
                      glslang
                      SPIRV
                      SPIRV-Tools
                      SPIRV-Tools-opt
                      OSDependent
                      OGLCompiler)


