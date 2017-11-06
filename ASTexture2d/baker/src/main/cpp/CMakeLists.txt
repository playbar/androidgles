cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wint-to-pointer-cast")

add_library(baker SHARED
            AndroidInterface.cpp
            ImageCell.cpp
            ImageInfo.cpp
            ImageManager.cpp
            ImageSize.cpp
            MojingTextureBaker.cpp
            )

# Include libraries needed for hello-jni lib
target_link_libraries(baker
                      android
                      GLESv3
                      log)
