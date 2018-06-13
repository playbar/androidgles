#ifndef __BAR_VULKAN_UTILS_JNI_H__
#define __BAR_VULKAN_UTILS_JNI_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_bar_vkview_VKUtilsLib_initVK(JNIEnv* env, jobject obj, jobject surface, jobject assetMgr);
JNIEXPORT void JNICALL Java_com_bar_vkview_VKUtilsLib_vkDrawFrame(JNIEnv* env, jobject obj);

JNIEXPORT jlong JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeCreate(JNIEnv *env, jclass type, jobject assetManager_,
                                   jstring vertexShader_, jstring fragmentShader_);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeRun(JNIEnv *env, jclass type, jlong nativeHandle, jobject surface);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativePause(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeResume(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeStop(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeStart(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeCreateSurface(JNIEnv *env, jclass type, jlong nativeHandle, jobject surface);


JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeOnSurfaceCreated(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeOnSurfaceChanged(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeOnDrawFrame(JNIEnv *env, jclass type, jlong nativeHandle);

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeCleanUp(JNIEnv *env, jclass type, jlong nativeHandle);


#ifdef __cplusplus
}
#endif


#endif

