#include <time.h>
#include <stdio.h>
#include "AndroidInterface.h"
#include "MojingTextureBaker.h"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
#if 0
	unsigned int testImageSize[] = {
		32,10,
		13,13,
		1,1,
		16, 32,
		80, 90,
		180, 230,
		100, 20,
		150, 100,
		20, 50,
		30, 30,// 10
		40, 50,
		70, 80,
		60, 20,
		80, 90,
		64, 53,
		110, 53,
		20, 122,
		30, 80,
		16, 100,
		56, 23,// 20
		89, 46,
		200, 79,
		23, 46,
		79, 66,
		13, 12,
		45, 68,
		77, 49,
		150, 46 ,
		0 , 0};
	struct timespec tp;
	unsigned long long Begin, End , LastTime;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	Begin = tp.tv_sec * 1000 + tp.tv_nsec * 1e-6;

	int iHandel = MojingTextureBakerAPI_BeginBaker(0);
	int iIndex = 0;
	int iLoop = 3;
	int iTextureID = 1;
	while (iTextureID < 800)
	{
		iIndex = 0;
		while (testImageSize[iIndex])
		{
			clock_gettime(CLOCK_MONOTONIC, &tp);
			End = tp.tv_sec * 1000 + tp.tv_nsec * 1e-6;

			MojingTextureBakerAPI_AddTexture(iHandel, iTextureID++, 0, 0, testImageSize[iIndex], testImageSize[iIndex + 1]);
			iIndex += 2;
			clock_gettime(CLOCK_MONOTONIC, &tp);
			LastTime = tp.tv_sec * 1000 + tp.tv_nsec * 1e-6 - End;
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &tp);
	End = tp.tv_sec * 1000 + tp.tv_nsec * 1e-6;
	unsigned long long totleTime = End - Begin;
	char *pszRects = MojingTextureBakerAPI_PrintRects(iHandel);
	char *pszCells = MojingTextureBakerAPI_PrintCells(iHandel);
	if (pszRects || pszCells)
	{
		FILE *pFile = fopen("/sdcard/MojingSDK/MojingTextureBaker.txt", "w+");
		if (pFile)
		{
			fprintf(pFile, "# ImageCount = %d , Totle Time = %d ms, LastPath = %d ms\n", iTextureID, (int)totleTime, (int)LastTime);
			if (pszRects)
				fputs(pszRects, pFile);
			fflush(pFile);

			if (pszCells)
			{
				if (pszRects)
					fputs("\n", pFile);

				fputs(pszCells, pFile);
			}
			fflush(pFile);
			fclose(pFile);
		}
		delete pszRects;
		delete pszCells;
	}
#endif
	return JNI_VERSION_1_6;
}

// 		函数功能：开始图片合批工作
// 		输入值： 合批后图片的最大尺寸，必须是2的整数次幂。默认值0 表示不限制。
// 		返回值： 合批工作组的句柄。是调用后续函数时必须传入的第一个参数。必须通过EndBaker函数进行释放。
int Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BeginBaker(JNIEnv* env, jobject thiz, int iMaxSize)
{
	return MojingTextureBakerAPI_BeginBaker(iMaxSize); 
}


// 		函数功能：向hHandle指定的合批工作组中添加一个图片。
// 		输入值：
// 		hHandle    和批工作组句柄，由BeginBaker函数返回。
// 		iTextureID  图片的ID
// 		iX ，iY ，iWidth ，iHeight 分别用来指定被合批的字图像在iTextureID中的范围，为了防止浮点数精度问题，所有变量都以像素为单位。
// 		返回值： 大于0时，表示加入此图像后合批图片的尺寸。否则表示加入失败。可能是由于超出了BeginBaker函数输入的iMaxSize的限制，或者重复添加。
int Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_AddTexture(JNIEnv* env, jobject thiz, int hHandle, int iTextureID, int iIndex, int iX, int iY, int iWidth, int iHeight)
{
	return MojingTextureBakerAPI_AddTexture(hHandle, iTextureID, iIndex, iX, iY, iWidth, iHeight);
}

// 		函数功能：向hHandle指定的合批工作组中删除一个图片。
// 		输入值：
// 		hHandle    和批工作组句柄，由BeginBaker函数返回。
// 		iTextureID  图片的ID。0表示删除所有图。
// 		iX ，iY ，iWidth ，iHeight 分别用来指定被合批的字图像在iTextureID中的范围，以像素为单位。如果iWidth iHeight均为0，表示删除所有iTextureID指定的图像。
// 		返回值： 大于0时，表示删除此图像后合批图片的尺寸。否则表示删除失败。可能是由于没有找到对应的图片。
int Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_RemoveTexture(JNIEnv* env, jobject thiz, int hHandle, int iTextureID, int iIndex, int iX, int iY, int iWidth, int iHeight)
{
	return MojingTextureBakerAPI_RemoveTexture(hHandle, iTextureID, iIndex, iX, iY, iWidth, iHeight);
}

// 		函数功能：获取hHandle指定的合批工作组合批后图片大小。
// 		输入值：
// 		hHandle    和批工作组句柄，由BeginBaker函数返回。
// 		返回值：
// 		大于0：          合批后图片大小
// 		等于0：          合批工作组为空，没有添加过任何图
// 		小于0：          没有找到对应的合批工作组
int Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_GetBakerTextureSize(JNIEnv* env, jobject thiz, int hHandle)
{
	return MojingTextureBakerAPI_GetBakerTextureSize(hHandle);
}


/*		函数功能：对hHandle指定的合批工作组执行合批。
输入值：
hHandle       和批工作组句柄，由BeginBaker函数返回。
iTextureID    合批输出图片的ID, 边长不得小于GetBakerTextureSize，且图片格式必须与所有子图像一致
bReplaceByBakedTexture 如果为Ture，表示用合批后的iTextureID和子图像在iTextureID中的坐标，替换子图像的参数
返回值：
jsonstring :
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
}*/
jstring Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_BakerTexture(JNIEnv* env, jobject thiz, int hHandle, int iTextureID, jboolean bReplaceByBakedTexture)
{
	jstring Ret;
	char * pRet = MojingTextureBakerAPI_BakerTexture(hHandle ,iTextureID, bReplaceByBakedTexture);

	if (pRet)
	{
		Ret = env->NewStringUTF(pRet);
		delete pRet;
	}
	else
	{
		Ret = env->NewStringUTF("");
	}
	return Ret;
}

/*
函数功能：结束hHandle指定的图片合批工作
输入值：
hHandle       和批工作组句柄，由BeginBaker函数返回。0表示删除所有的Baker对象。
返回值： 无。
*/
void Java_com_baofeng_mojing_MojingTextureBaker_MojingTextureBaker_EndBaker(JNIEnv* env, jobject thiz, int hHandle)
{
	MojingTextureBakerAPI_EndBaker(hHandle);
}

void Java_com_baofeng_mojing_MojingTextureBaker_SaveToImage(JNIEnv* env, jobject thiz, int iTextureID, int iWidth, int iHeight, jstring szImageFileName)
{
	MojingTextureBakerAPI_SaveImage(iTextureID, iWidth, iHeight, "/sdcard/MojingSDK/Test.bmp");
}