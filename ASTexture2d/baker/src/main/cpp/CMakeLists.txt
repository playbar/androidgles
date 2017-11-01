cmake_minimum_required(VERSION 3.4.1)

add_library(MojingTextureBaker SHARED
            AndroidInterface.cpp
            ImageCell.cpp
            ImageInfo.cpp
            ImageManager.cpp
            ImageSize.cpp
            MojingTextureBaker.cpp
            )

# Include libraries needed for hello-jni lib
target_link_libraries(MojingTextureBaker
                      android
                      GLESv3
                      log)
