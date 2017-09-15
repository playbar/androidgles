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
            Simple_Texture2D.cpp
            DynamicLibrary.cpp
            )


target_include_directories(glesbook PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/core/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/core/libpixelflinger/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/libhardware/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/ics/include
                          )

add_library(lib_ui SHARED IMPORTED )
set_target_properties(lib_ui PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libui.so)

# Include libraries needed for hello-jni lib
target_link_libraries(glesbook
                      android
                      log
                      EGL
                      GLESv3)
