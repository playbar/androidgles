#pragma once
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <map>
#include "MojingTextureBaker.h"

extern "C"
{
	/*******************************************************************************************
	*函数功能：开始图片合批工作*
	*输入值： 合批后图片的最大尺寸，必须是2的整数次幂。默认值0 表示不限制。
	*返回值： 合批工作组的句柄。是调用后续函数时必须传入的第一个参数。必须通过EndBaker函数进行释放。
	********************************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BeginBaker(JNIEnv* env, jobject thiz,
		jint iMaxSize);

	/****************************************************************************************
	* 函数功能：向hHandle指定的合批工作组中添加一个图片。
	*输入值：
	*hHandle    和批工作组句柄，由BeginBaker函数返回。
	*iTextureID  图片的ID
	*iX ，iY ，iWidth ，iHeight 分别用来指定被合批的字图像在iTextureID中的范围，以像素为单位。
	*返回值： 大于0时，表示加入此图像后合批图片的尺寸。否则表示加入失败。
	*可能是由于超出了BeginBaker函数输入的iMaxSize的限制，或者重复添加。
	****************************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_AddTexture(JNIEnv* env, jobject thiz,
		jint hHandle, jint iTextureID, jint iX, jint iY, jint iWidth, jint iHeight);

	/************************************************************************
	*函数功能：向hHandle指定的合批工作组中删除一个图片。
	*输入值：
	*hHandle    和批工作组句柄，由BeginBaker函数返回。
	*iTextureID  图片的ID。0表示删除所有图。
	*iX ，iY ，iWidth ，iHeight 分别用来指定被合批的字图像在iTextureID中的范围，以像素为单位。
	*如果iX iY iWidth iHeight均为0，表示删除所有iTextureID指定的图像。
	*返回值： 大于0时，表示删除此图像后合批图片的尺寸。否则表示删除失败。可能是由于没有找到对应的图片。
	************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_RemoveTexture(JNIEnv* env, jobject thiz,
		jint hHandle, jint iTextureID, jint iX, jint iY, jint iWidth, jint iHeight);

	/************************************************************************
	*函数功能：获取hHandle指定的合批工作组合批后图片大小。
	*输入值：
	*hHandle    和批工作组句柄，由BeginBaker函数返回。
	*返回值：
	*大于0：          合批后图片大小
	*等于0：          合批工作组为空，没有添加过任何图
	*小于0：          没有找到对应的合批工作组
	************************************************************************/
	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GetBakerTextureSize(JNIEnv* env, jobject thiz,
		jint hHandle);

	/************************************************************************
	*函数功能：对hHandle指定的合批工作组执行合批。
	*输入值：
	*hHandle       和批工作组句柄，由BeginBaker函数返回。
	*iTextureID    合批输出图片的ID, 边长不得小于GetBakerTextureSize，且图片格式必须与所有子图像一致
	*bReplaceByBakedTexture 如果为Ture，表示用合批后的iTextureID和子图像在iTextureID中的坐标，替换子图像的参数
	*返回值：
	jsonstring:
	{
	SubTextureCount: N,                    // 子图像的总数
	BakedTextureSize : Size,                // 合批后图像的变长
	BakedTextureID : iTextureID,         // 合批后图像的ID
	BakedTexture : [
	{
	SubTextureID:ID,    // 原始图像ID
	Position : [X, Y],         //在iTextureID中的左上角坐标
	Size : [W, H]              // 宽度、高度
	},
	...]
	}
	************************************************************************/
	JNIEXPORT jstring JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BakerTexture(JNIEnv* env, jobject thiz,
		jint hHandle, jint iTextureID, jboolean bReplaceByBakedTexture);

	/************************************************************************
	*函数功能：结束hHandle指定的图片合批工作
	*输入值：
	*hHandle       和批工作组句柄，由BeginBaker函数返回。0表示删除所有的Baker对象。
	*返回值： 无。
	************************************************************************/
	JNIEXPORT void JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_EndBaker(JNIEnv* env, jobject thiz,
		jint hHandle);


	JNIEXPORT void JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_OpenGLTest(JNIEnv* env, jobject thiz);

	JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GLTest(JNIEnv* env, jobject thiz,
			jint iMaxSize);

	
}