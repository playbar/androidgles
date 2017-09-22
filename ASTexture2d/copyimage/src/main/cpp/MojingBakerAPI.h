#pragma once
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <map>
#include "MojingTextureBaker.h"

extern "C"
{
	/*******************************************************************************************
	*�������ܣ���ʼͼƬ��������*
	*����ֵ�� ������ͼƬ�����ߴ磬������2���������ݡ�Ĭ��ֵ0 ��ʾ�����ơ�
	*����ֵ�� ����������ľ�����ǵ��ú�������ʱ���봫��ĵ�һ������������ͨ��EndBaker���������ͷš�
	********************************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BeginBaker(JNIEnv* env, jobject thiz,
		jint iMaxSize);

	/****************************************************************************************
	* �������ܣ���hHandleָ���ĺ��������������һ��ͼƬ��
	*����ֵ��
	*hHandle    ����������������BeginBaker�������ء�
	*iTextureID  ͼƬ��ID
	*iX ��iY ��iWidth ��iHeight �ֱ�����ָ������������ͼ����iTextureID�еķ�Χ��������Ϊ��λ��
	*����ֵ�� ����0ʱ����ʾ�����ͼ������ͼƬ�ĳߴ硣�����ʾ����ʧ�ܡ�
	*���������ڳ�����BeginBaker���������iMaxSize�����ƣ������ظ���ӡ�
	****************************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_AddTexture(JNIEnv* env, jobject thiz,
		jint hHandle, jint iTextureID, jint iX, jint iY, jint iWidth, jint iHeight);

	/************************************************************************
	*�������ܣ���hHandleָ���ĺ�����������ɾ��һ��ͼƬ��
	*����ֵ��
	*hHandle    ����������������BeginBaker�������ء�
	*iTextureID  ͼƬ��ID��0��ʾɾ������ͼ��
	*iX ��iY ��iWidth ��iHeight �ֱ�����ָ������������ͼ����iTextureID�еķ�Χ��������Ϊ��λ��
	*���iX iY iWidth iHeight��Ϊ0����ʾɾ������iTextureIDָ����ͼ��
	*����ֵ�� ����0ʱ����ʾɾ����ͼ������ͼƬ�ĳߴ硣�����ʾɾ��ʧ�ܡ�����������û���ҵ���Ӧ��ͼƬ��
	************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_RemoveTexture(JNIEnv* env, jobject thiz,
		jint hHandle, jint iTextureID, jint iX, jint iY, jint iWidth, jint iHeight);

	/************************************************************************
	*�������ܣ���ȡhHandleָ���ĺ��������������ͼƬ��С��
	*����ֵ��
	*hHandle    ����������������BeginBaker�������ء�
	*����ֵ��
	*����0��          ������ͼƬ��С
	*����0��          ����������Ϊ�գ�û����ӹ��κ�ͼ
	*С��0��          û���ҵ���Ӧ�ĺ���������
	************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GetBakerTextureSize(JNIEnv* env, jobject thiz,
		jint hHandle);

	/************************************************************************
	*�������ܣ���hHandleָ���ĺ���������ִ�к�����
	*����ֵ��
	*hHandle       ����������������BeginBaker�������ء�
	*iTextureID    �������ͼƬ��ID, �߳�����С��GetBakerTextureSize����ͼƬ��ʽ������������ͼ��һ��
	*bReplaceByBakedTexture ���ΪTure����ʾ�ú������iTextureID����ͼ����iTextureID�е����꣬�滻��ͼ��Ĳ���
	*����ֵ��
	jsonstring:
	{
	SubTextureCount: N,                    // ��ͼ�������
	BakedTextureSize : Size,                // ������ͼ��ı䳤
	BakedTextureID : iTextureID,         // ������ͼ���ID
	BakedTexture : [
	{
	SubTextureID:ID,    // ԭʼͼ��ID
	Position : [X, Y],         //��iTextureID�е����Ͻ�����
	Size : [W, H]              // ��ȡ��߶�
	},
	...]
	}
	************************************************************************/
	JNIEXPORT jstring JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BakerTexture(JNIEnv* env, jobject thiz,
		jint hHandle, jint iTextureID, jboolean bReplaceByBakedTexture);

	/************************************************************************
	*�������ܣ�����hHandleָ����ͼƬ��������
	*����ֵ��
	*hHandle       ����������������BeginBaker�������ء�0��ʾɾ�����е�Baker����
	*����ֵ�� �ޡ�
	************************************************************************/
	JNIEXPORT void JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_EndBaker(JNIEnv* env, jobject thiz,
		jint hHandle);


	JNIEXPORT void JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_OpenGLTest(JNIEnv* env, jobject thiz);

	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GLTest(JNIEnv* env, jobject thiz,
			jint iMaxSize);

	
}