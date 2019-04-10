cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fexceptions")


add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DRAPIDJSON_HAS_CXX11_RVALUE_REFS=1)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/assimp)

#ModelAssimpNative
add_library(ModelAssimpNative SHARED
            jniCalls/assimpActivity.cpp
            jniCalls/gestureClass.cpp
            jniCalls/myGLRenderer.cpp
            modelAssimp/assimpLoader.cpp
            modelAssimp/misc.cpp
            modelAssimp/myGLCamera.cpp
            modelAssimp/myGLFunctions.cpp
            modelAssimp/myJNIHelper.cpp
            modelAssimp/myShader.cpp
            modelAssimp/modelAssimp.cpp
            ndk_helper/gestureDetector.cpp
            ndk_helper/gl3stub.c
            ndk_helper/GLContext.cpp
            ndk_helper/interpolator.cpp
            ndk_helper/JNIHelper.cpp
            ndk_helper/perfMonitor.cpp
            ndk_helper/shader.cpp
            ndk_helper/tapCamera.cpp
            ndk_helper/vecmath.cpp

            )

target_include_directories(ModelAssimpNative PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include
                          ${CMAKE_CURRENT_SOURCE_DIR}/externals/glm
                          ${CMAKE_CURRENT_SOURCE_DIR}/externals/opencv
                          ${CMAKE_CURRENT_SOURCE_DIR}/jniCalls
                          ${CMAKE_CURRENT_SOURCE_DIR}/modelAssimp
                          ${CMAKE_CURRENT_SOURCE_DIR}/ndk_helper
                     )

add_library(lib_assimp SHARED IMPORTED)
set_target_properties(lib_assimp PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libassimp_bin.so)

add_library(lib_opencv SHARED IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libopencv_java3.so)


target_link_libraries(ModelAssimpNative
                      android
                      log
                      EGL
                      GLESv2
                      assimp
                      lib_opencv
                  )


