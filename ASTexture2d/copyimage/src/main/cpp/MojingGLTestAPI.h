#pragma once
#include <jni.h>
#include <stdio.h>
#include <GLES3/gl3.h>

extern "C"
{
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingGLSurfaceViewRender_GLTest(JNIEnv* env, jobject thiz,
		jint iTextureId);
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingGLSurfaceViewRender_GetTextureId(JNIEnv* env, jobject thiz);
}