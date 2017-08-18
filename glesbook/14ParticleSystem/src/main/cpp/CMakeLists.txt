cmake_minimum_required(VERSION 3.4.1)

add_library(glesbook SHARED
            android_native_app_glue.c
            esShader.c
            esShapes.c
            esTransform.c
            esUtil.c
            esUtil_Android.c
            ParticleSystemTransformFeedback.c
            Noise3D.c)

# Include libraries needed for hello-jni lib
target_link_libraries(glesbook
                      android
                      log
                      EGL
                      GLESv3)
