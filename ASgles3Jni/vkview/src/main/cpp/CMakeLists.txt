cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                    -std=c++11 -fexceptions -Wall -fno-rtti -Wno-error=format-security \
                    -DVK_USE_PLATFORM_ANDROID_KHR")

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

add_library(vklib SHARED
           vk/vulkan_tutorial.cpp
           vk/vulkan_wrapper.cpp
            )

target_include_directories(vklib PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/vk
                          ${CMAKE_CURRENT_SOURCE_DIR}/data
                          ${CMAKE_CURRENT_SOURCE_DIR}/stb
                          ${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader
                          )

target_link_libraries(vklib
                      android
                      log
                      GLESv3
                      EGL )


