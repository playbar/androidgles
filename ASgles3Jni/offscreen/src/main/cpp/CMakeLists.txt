cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
#                    -std=c++11 -fno-exceptions -Wall -fno-rtti -Wno-error=format-security \")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-omit-frame-pointer -fPIC -Wa,-mimplicit-it=thumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}   -fno-omit-frame-pointer  -Wa,-mimplicit-it=thumb")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ")

ADD_DEFINITIONS(-DNK_INCLUDE_FIXED_TYPES)
ADD_DEFINITIONS(-DNK_INCLUDE_DEFAULT_ALLOCATOR)
ADD_DEFINITIONS(-DNK_INCLUDE_STANDARD_IO)
ADD_DEFINITIONS(-DNK_INCLUDE_VERTEX_BUFFER_OUTPUT)
ADD_DEFINITIONS(-DNK_INCLUDE_FONT_BAKING)
ADD_DEFINITIONS(-DNK_INCLUDE_DEFAULT_FONT)
ADD_DEFINITIONS(-DGLES)

add_library(mygles SHARED
           OffScreenTest.cpp
            )

target_link_libraries(mygles
                      android
                      log
                      GLESv2
                      EGL )


