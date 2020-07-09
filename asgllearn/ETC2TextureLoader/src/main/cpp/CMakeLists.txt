cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#                    -std=c++11 -fno-exceptions -Wall -fno-rtti -Wno-error=format-security \")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-omit-frame-pointer -fPIC -Wa,-mimplicit-it=thumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -fno-omit-frame-pointer  -Wa,-mimplicit-it=thumb")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ")

ADD_DEFINITIONS(-DKTX_OPENGL_ES3=1)
ADD_DEFINITIONS(-DSUPPORT_SOFTWARE_ETC_UNPACK=0)

add_library(textureloader SHARED
           jni_main.c
           file.c
           texture.c
           stb/stb_image.c
           libktx/checkheader.c
           libktx/hashtable.c
           libktx/loader.c
           libktx/swap.c
           libktx/writer.c
            )

target_include_directories(textureloader PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/libktx
            ${CMAKE_CURRENT_SOURCE_DIR}/stb)

target_link_libraries(textureloader
                      android
                      log
                      GLESv3
                      EGL )



