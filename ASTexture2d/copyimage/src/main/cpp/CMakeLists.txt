cmake_minimum_required(VERSION 3.4.1)

add_library(MojingTextureBaker SHARED
            MojingBaker.cpp
            MojingBakerAPI.cpp
            MojingGLTestAPI.cpp
            MojingTextureBaker.cpp
            load.c)

# Include libraries needed for hello-jni lib
target_link_libraries(MojingTextureBaker
                      android
                      GLESv3
                      log)
