#include "MojingBakerAPI.h"
#include "MojingBaker.h"
#include <GLES3/gl3.h>

JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BeginBaker(JNIEnv* env, jobject thiz,
	jint iMaxSize)
{

	return MojingBaker::GetInstance()->BeginBaker(iMaxSize);
}

JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_AddTexture(JNIEnv* env, jobject thiz,
	jint hHandle, jint iTextureID, jint iX, jint iY, jint iWidth, jint iHeight)
{
	return MojingBaker::GetInstance()->AddTexture(hHandle, iTextureID, iX, iY, iWidth, iHeight);
}

JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_RemoveTexture(JNIEnv* env, jobject thiz,
	jint hHandle, jint iTextureID, jint iX, jint iY, jint iWidth, jint iHeight)
{
	return MojingBaker::GetInstance()->RemoveTexture(hHandle, iTextureID);
}

JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GetBakerTextureSize(JNIEnv* env, jobject thiz,
	jint hHandle)
{
	return MojingBaker::GetInstance()->GetBakerTextureSize(hHandle);
}

JNIEXPORT jstring JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BakerTexture(JNIEnv* env, jobject thiz, 
	jint hHandle, jint iTextureID, jboolean bReplaceByBakedTexture)
{
	jstring jsRet = env->NewStringUTF("test");
//    jstring jsRet;
	char *pRet = MojingBaker::GetInstance()->BakerTexture(hHandle, iTextureID);
	if (NULL != pRet)
	{
		jsRet = env->NewStringUTF(pRet);
//		jclass strClass = env->FindClass("java/lang/String");
//		jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
//		jbyteArray bytes = env->NewByteArray(strlen(pRet));
//		env->SetByteArrayRegion(bytes, 0, strlen(pRet), (jbyte*)pRet);
//		jstring encoding = env->NewStringUTF("utf-8");
//		jsRet = (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
	}
	return jsRet;
}


JNIEXPORT void JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_EndBaker(JNIEnv* env, jobject thiz,
	jint hHandle)
{
	MojingBaker::GetInstance()->EndBaker(hHandle);
	//LOGI("MojingBaker:EndBaker---HandleID:%d", hHandle);
}

JNIEXPORT void JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_OpenGLTest(JNIEnv* env, jobject thiz)
{
	//GLuint uiFramBuffer;
	//glGenFramebuffers(1, &uiFramBuffer);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, uiFramBuffer);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDeleteBuffers(1, &uiFramBuffer);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glViewport(0, 0, 100, 100);

}

JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GLTest(JNIEnv* env, jobject thiz,
	jint iMaxSize)
{
	//MojingTextureBakerAPI_SaveToImage(iMaxSize, 144, 144, "1.png");
	return 0;
}
