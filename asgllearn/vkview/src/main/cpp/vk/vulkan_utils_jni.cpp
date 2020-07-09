#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>

#include "vulkan_utils.h"
#include "vulkan_utils_jni.h"
#include "mylog.h"

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_initVK(JNIEnv* env, jobject obj, jobject surface, jobject assetMgr)
{
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if(window == NULL ){
        LOGE("window surface fail");
    }
    AAssetManager* mgr = AAssetManager_fromJava(env, assetMgr);
    ANativeWindow_acquire(window);
//    InitVulkan(window, mgr);
    ANativeWindow_release(window);
    return;

}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_vkDrawFrame(JNIEnv* env, jobject obj)
{
//    while( true ) {
//        if (IsVulkanReady()) {
//            VulkanDrawFrame();
//        }
//    }
}

JNIEXPORT jlong JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeCreate(
        JNIEnv *env, jclass type, jobject assetManager_, jstring vertexShader_,
        jstring fragmentShader_) {

    AAssetManager *assetManager = AAssetManager_fromJava(env, assetManager_);
    if (assetManager == nullptr) {
        LOGE("get assetManager fail!");
        return 0;
    }

    const char *vertexShader = env->GetStringUTFChars(vertexShader_, 0);
    const char *fragmentShader = env->GetStringUTFChars(fragmentShader_, 0);

    VulkanUtils *app = new VulkanUtils(assetManager, vertexShader, fragmentShader);

    env->ReleaseStringUTFChars(vertexShader_, vertexShader);
    env->ReleaseStringUTFChars(fragmentShader_, fragmentShader);

    return (jlong) app;
}


JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativePause(JNIEnv *env, jclass type,
                                        jlong nativeHandle) {
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->pause();
}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeResume(JNIEnv *env, jclass type, jlong nativeHandle) {
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->resume();
}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeStop(JNIEnv *env, jclass type, jlong nativeHandle) {
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->stop();
}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeStart(JNIEnv *env, jclass type, jlong nativeHandle)
{
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->start();
}


JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeCreateSurface(JNIEnv *env, jclass type, jlong nativeHandle, jobject surface)
{
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("get window from surface fail!");
        return;
    }

    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->initWindow(window);
    app->createSurfaceDevice();
    return;
}



JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeOnSurfaceCreated(JNIEnv *env, jclass type, jlong nativeHandle)
{
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->OnSurfaceCreated();
    return;
}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeOnSurfaceChanged(JNIEnv *env, jclass type, jlong nativeHandle)
{
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->OnSurfaceChanged();
    return;
}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeOnDrawFrame(JNIEnv *env, jclass type, jlong nativeHandle)
{
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->OnDrawFrame();
    return;
}

JNIEXPORT void JNICALL
Java_com_bar_vkview_VKUtilsLib_nativeCleanUp(JNIEnv *env, jclass type, jlong nativeHandle)
{
    VulkanUtils *app = reinterpret_cast<VulkanUtils *>(nativeHandle);
    app->cleanUp();
    return;
}
