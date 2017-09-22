#include "MojingGLTestAPI.h"
#include <vector>
int texture_id = 0;

std::vector<int> v_TextureIdVector;


JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingGLSurfaceViewRender_GLTest(JNIEnv* env, jobject thiz,
	jint iTextureId)
{
	v_TextureIdVector.push_back(iTextureId);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 100, 100);
	return 0;
}

JNIEXPORT jint JNICALL Java_com_baofeng_mojing_MojingTextureBaker_MojingGLSurfaceViewRender_GetTextureId(JNIEnv* env, jobject thiz)
{
	//MojingTextureBakerAPI_SaveToImage(texture_id, 144, 144, "/sdcard/MojingSDK/1.png");
	GLuint uiFramebuffer;
	glGenFramebuffers(1, &uiFramebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, v_TextureIdVector[0], 0);
	int size = v_TextureIdVector.size();
	return v_TextureIdVector[0];
}













//void MojingTextureBakerAPI_SaveToImageRGBA(const unsigned char * pBuffer, int iWidth, int iHeight, const char *szImageFileName)
//{
//	unsigned char BitmapFileheader[] = {
//		0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
//		0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
//		0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//	int iFileSizeOffset = 2;// FILE SIZE
//	int iWidthOffset = 0x12;
//	int iHeightOffset = iWidth + 4;
//
//	int iDataSize = iWidth * iHeight * 4;
//	int iFileDataSize = 0x36 + iDataSize;
//
//	*((int *)(BitmapFileheader + iFileSizeOffset)) = iFileDataSize;
//	*((int *)(BitmapFileheader + iWidthOffset)) = iWidth;
//	*((int *)(BitmapFileheader + iHeightOffset)) = iHeight;
//
//
//	FILE *pFile = fopen(szImageFileName, "wb+");
//	if (pFile)
//	{
//		fwrite(BitmapFileheader, 1, 0x36, pFile);
//		int iWrited = 0;
//		while (iWrited < iFileDataSize)
//		{
//			int iThisWrite = (1024 * 512) < (iFileDataSize - iWrited) ? (1024 * 512) : (iFileDataSize - iWrited);
//			//int iThisWrite = fmin(1024 * 512, iFileDataSize - iWrited);
//			fwrite(pBuffer + iWrited, 1, iThisWrite, pFile);
//			fflush(pFile);
//			iWrited += iThisWrite;
//		}
//		fclose(pFile);
//	}
//}
//
//// 4 ����ͼƬ ��İ취�����Դ���������ݿ�������
//void MojingTextureBakerAPI_SaveToImage(int iTextureID, int iWidth, int iHeight, const char *szImageFileName)
//{
//	// ���� FBO
//	unsigned int fbo;
//	glGenFramebuffers(1, &fbo);
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
//	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iTextureID, 0);
//
//	// ѡ��ʹ�õ�ͼƬ
//	glBindTexture(GL_TEXTURE_2D, iTextureID);
//	// ��ȡ����
//	unsigned char *pPixelData = new unsigned char[iWidth * iHeight * 4];
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//	glReadPixels(0, 0, iWidth, iHeight,
//		GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	MojingTextureBakerAPI_SaveToImageRGBA(pPixelData, iWidth, iHeight, szImageFileName);
//
//}