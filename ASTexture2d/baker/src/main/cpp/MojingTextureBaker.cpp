/*
 * Created by VisualGDB. Based on hello-jni example.

 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "MojingTextureBaker.h"
#include "ImageManager.h"

int s_ButtonPressCounter = 0;
typedef vector<CImageManager*> CImageManagerArray;

CImageManagerArray g_ImageManagerArray;
bool GL_CheckErrors(const char * logTitle);
// 		函数功能：开始图片合批工作
// 		输入值： 合批后图片的最大尺寸，必须是2的整数次幂。默认值0 表示不限制。
// 		返回值： 合批工作组的句柄。是调用后续函数时必须传入的第一个参数。必须通过EndBaker函数进行释放。
HANDLE MojingTextureBakerAPI_BeginBaker(int iMaxSize)
{
	CImageManager *pNewImageManager = new  CImageManager;
	pNewImageManager->SetMaxSize(iMaxSize);
	g_ImageManagerArray.push_back(pNewImageManager);
	return (HANDLE)pNewImageManager;
}


// 		函数功能：向hHandle指定的合批工作组中添加一个图片。
// 		输入值：
// 		hHandle    和批工作组句柄，由BeginBaker函数返回。
// 		iTextureID  图片的ID
// 		iX ，iY ，iWidth ，iHeight 分别用来指定被合批的字图像在iTextureID中的范围，为了防止浮点数精度问题，所有变量都以像素为单位。
// 		返回值： 大于0时，表示加入此图像后合批图片的尺寸。否则表示加入失败。可能是由于超出了BeginBaker函数输入的iMaxSize的限制，或者重复添加。
int MojingTextureBakerAPI_AddTexture(HANDLE hHandle, TEXTURE_ID iTextureID, int index, int iX, int iY, int iWidth, int iHeight)
{
	int iRet = 0;
	
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		CImageInfo *pNewImage = new CImageInfo(iX, iY, iWidth, iHeight , iTextureID, index);
		
		if (pImageManager->m_ImageArray.Find(pNewImage) < 0)
		{
			int iPos = pImageManager->m_ImageArray.AddImage(pNewImage);
			// pImageManager->BakeImage();
// 			if (pImageManager->GetRoot()->GetSize().GetWidth() <= pImageManager->GetMaxSize() || 0 == pImageManager->GetMaxSize() )
// 			{
// 				iRet = pImageManager->GetRoot()->GetSize().GetWidth();
// 			}
// 			else
// 			{// 超大了
// 				pImageManager->m_ImageArray.RemoveImage(iPos);
// 				delete pNewImage;
// 				pImageManager->BakeImage();
// 				return -100;
// 			}
			return 1;
		}
		else
		{
			iRet = - 2;
		}
	}
	return iRet;
}

// 		函数功能：向hHandle指定的合批工作组中删除一个图片。
// 		输入值：
// 		hHandle    和批工作组句柄，由BeginBaker函数返回。
// 		iTextureID  图片的ID。0表示删除所有图。
// 		iX ，iY ，iWidth ，iHeight 分别用来指定被合批的字图像在iTextureID中的范围，以像素为单位。如果iWidth iHeight均为0，表示删除所有iTextureID指定的图像。
// 		返回值： 大于0时，表示删除此图像后合批图片的尺寸。否则表示删除失败。可能是由于没有找到对应的图片。
int MojingTextureBakerAPI_RemoveTexture(HANDLE hHandle, TEXTURE_ID iTextureID, int index, int iX, int iY, int iWidth, int iHeight)
{
	int iRet = 0;
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		if (iWidth != 0 && iHeight != 0)
		{
			int iPos = pImageManager->m_ImageArray.Find(iTextureID, index, iX, iY, iWidth, iHeight);
			if (iPos >= 0)
			{
				pImageManager->m_ImageArray.RemoveImage(iPos);
				iRet = pImageManager->CalculationImagePosition();
			}
			else
			{
				iRet = -2;
			}
		}
		else// iWidth != 0 && iHeight != 0 , 
		{// 删除所有iTextureID
			bool bDeleted = false;
			int iPos = pImageManager->m_ImageArray.Find(iTextureID);
			if (iPos >= 0)
			{
				bDeleted = true;
				do 
				{
					pImageManager->m_ImageArray.RemoveImage(iPos);
					iPos = pImageManager->m_ImageArray.Find(iTextureID);
				} while (iPos >= 0);
			}
			else
			{
				iRet = -2;
			}
			iRet = pImageManager->CalculationImagePosition();
		}
	}
	else
	{
		iRet = -1;
	}
	return iRet;
}

// 		函数功能：获取hHandle指定的合批工作组合批后图片大小。
// 		输入值：
// 		hHandle    和批工作组句柄，由BeginBaker函数返回。
// 		返回值：
// 		大于0：          合批后图片大小
// 		等于0：          合批工作组为空，没有添加过任何图
// 		小于0：          没有找到对应的合批工作组
int MojingTextureBakerAPI_GetBakerTextureSize(HANDLE hHandle, FP_UpdateProgressInfo pUpdateProgressInfo)
{
	int iRet = 0;
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		if (pImageManager->GetRoot())
		{
			iRet = pImageManager->GetRoot()->GetSize().GetWidth();
		}
		else
		{
			pImageManager->CalculationImagePosition(pUpdateProgressInfo);
			if (pImageManager->GetRoot())
				iRet = pImageManager->GetRoot()->GetSize().GetWidth();
			else
				iRet = -2;
		}
	}
	else
	{
		iRet = -1;
	}
	return iRet;
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
char* MojingTextureBakerAPI_BakerTexture(HANDLE hHandle, TEXTURE_ID iTextureID, bool bReplaceByBakedTexture, FP_UpdateProgressInfo pUpdateProgressInfo)
{
	char* pRet = NULL;
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		if (pImageManager->m_ImageArray.GetSize() && pImageManager->GetRoot())
		{
			int iElementSize = 128;
#ifdef WIN32
			iElementSize += MAX_PATH;
#endif
			int iBufferSize = 1024 + pImageManager->m_ImageArray.GetSize() * iElementSize;
			char *pScriptBuffer = new char[iBufferSize];
			*pScriptBuffer = 0;
			char *pPrintBegin = pScriptBuffer;
			int iCount = pImageManager->m_ImageArray.GetSize();
#ifndef WIN32
			pPrintBegin += sprintf(pPrintBegin, "{\n\"SubTextureCount\":%d,\n\"BakedTextureSize\":%d, \n\"BakedTextureID\":%d,\n\"BakedTexture\":[\n",
				iCount, pImageManager->GetRoot()->GetSize().GetWidth(), iTextureID);
#else
			//替换掉\为\\;
			string tmpFileName = iTextureID.m_szFileName;
			replace_all_distinct(tmpFileName, "\\", "\\\\");
			pPrintBegin += sprintf(pPrintBegin, "{\n\"SubTextureCount\":%d,\n\"BakedTextureSize\":%d, \n\"BakedTextureID\":\"%s\",\n\"BakedTexture\":[\n",
				iCount, pImageManager->GetRoot()->GetSize().GetWidth(), tmpFileName.c_str());
#endif // !WIN32

			
			
			/*按照TextureID分组，这样可以提高效率*/
#ifndef WIN32
			typedef map<TEXTURE_ID, vector<CImageInfo*> > CBakerMap;
			CBakerMap BakerMap;
#else
			ProgressInfo PI;
			if (pUpdateProgressInfo)
			{
				PI.m_iProgressNow = 0;
				PI.m_iProgressMax = iCount;
				pUpdateProgressInfo(&PI);
			}
			Graphics GR(iTextureID.m_pImage);
			GR.Clear(Color(0, 0, 0, 0));
#endif
			
			for (int iIndex = 0; iIndex < iCount; iIndex++)
			{
				CImageInfo* pImage = pImageManager->m_ImageArray[iIndex];
				pPrintBegin += pImage->PrintScript(pPrintBegin);
				if (iIndex < iCount - 1)
				{
					*pPrintBegin++= ',';
					*pPrintBegin++ = '\n';
					*pPrintBegin = 0;
				}
#ifndef WIN32
				TEXTURE_ID ThisFindKey  = pImage->GetTextureID();


				CBakerMap::iterator it = BakerMap.find(ThisFindKey);
				if (BakerMap.end() == it)
				{
					vector<CImageInfo*> NewGroup;
					NewGroup.push_back(pImage);
					BakerMap[ThisFindKey] = NewGroup;
				}
				else
				{
					BakerMap[ThisFindKey].push_back(pImage);
				}
#else
				if (pUpdateProgressInfo)
				{
					strcpy(PI.m_szProgressText, pImage->GetTextureID().m_szFileName.c_str());
					PI.m_iProgressNow++;
					pUpdateProgressInfo(&PI);
				}
				pImage->DoBaker(&GR);
#endif
			} // end of first for
			strcat(pPrintBegin , "]}");
			pRet = pScriptBuffer;
#ifndef WIN32
			if (glIsTexture(iTextureID))
			{
				unsigned int fbo;
				glGenFramebuffers(1, &fbo);
				glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
//				glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_BLEND);
//				glClearColor(0, 0, 0, 0);
//				glClear(GL_COLOR_BUFFER_BIT);
				int iLastBakedTextureID = -1;
				for (CBakerMap::iterator it = BakerMap.begin(); it != BakerMap.end(); it++)
				{
					for (int iImage = 0; iImage < it->second.size(); iImage++)
					{
						CImageInfo *pImage = it->second[iImage];
						pImage->DoBaker(iTextureID, iLastBakedTextureID);
						iLastBakedTextureID = pImage->GetTextureID();
						if (bReplaceByBakedTexture)
							pImage->UpdateBakedPosition(iTextureID);
					}
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
				glDeleteBuffers(1, &fbo);
			} // if (glIsTexture(iTextureID))
#else 
			GR.Flush();
#endif
		}// if (pImageManager->m_ImageArray.GetSize() && pImageManager->GetRoot())
	}
	return pRet;
}
char * MojingTextureBakerAPI_PrintRects(HANDLE hHandle)
{
	char * pRet = NULL;
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		pRet = pImageManager->PrintRects();
	}
	return pRet;
}
char * MojingTextureBakerAPI_PrintCells(HANDLE hHandle)
{
	char * pRet = NULL;
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		pRet = pImageManager->PrintCells();
	}
	return pRet;
}
char * MojingTextureBakerAPI_PrintJson(HANDLE hHandle , int iTextureID)
{
	char * pRet = NULL;
	if (find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle) != g_ImageManagerArray.end())
	{
		CImageManager* pImageManager = (CImageManager*)hHandle;
		int iBufferSize = 1024 + pImageManager->m_ImageArray.GetSize() * 128;
		char *pScriptBuffer = new char[iBufferSize];
		*pScriptBuffer = 0;
		char *pPrintBegin = pScriptBuffer;
		int iCount = pImageManager->m_ImageArray.GetSize();
		pPrintBegin += sprintf(pPrintBegin, "{\nSubTextureCount:%d,\nBakedTextureSize:%d, \nBakedTextureID:%d,\nBakedTexture:[\n",
			iCount, pImageManager->GetRoot()->GetSize().GetWidth(), iTextureID);
		
		for (int iIndex = 0; iIndex < iCount; iIndex++)
		{
			CImageInfo* pImage = pImageManager->m_ImageArray[iIndex];
			pPrintBegin += pImage->PrintScript(pPrintBegin);
			if (iIndex < iCount - 1)
			{
				*pPrintBegin++ = ',';
				*pPrintBegin++ = '\n';
				*pPrintBegin = 0;
			}
		}
		*pPrintBegin++ = ']';
		*pPrintBegin++ = '}';
		*pPrintBegin = 0;
		pRet = pPrintBegin;
	}
	return pRet;
}

/*
函数功能：结束hHandle指定的图片合批工作
输入值：
hHandle       和批工作组句柄，由BeginBaker函数返回。0表示删除所有的Baker对象。
返回值： 无。
*/
void MojingTextureBakerAPI_EndBaker(HANDLE hHandle)
{
	if (hHandle)
	{
		CImageManagerArray::iterator it = find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle);
		if (it != g_ImageManagerArray.end())
		{
			CImageManager* pImageManager = (CImageManager*)hHandle;
			delete pImageManager;
			g_ImageManagerArray.erase(it);
		}
	}
	else
	{
		//hHandle = 0 , 全删掉
		for (CImageManagerArray::iterator it = g_ImageManagerArray.begin(); it != g_ImageManagerArray.end(); it++)
		{
			delete *it;
		}
		g_ImageManagerArray.clear();
	}
}


void MojingTextureBakerAPI_SaveToFile(HANDLE hHandle, const char *szScriptFileName, const char *szImageFileName)
{
	if (hHandle)
	{
		CImageManagerArray::iterator it = find(g_ImageManagerArray.begin(), g_ImageManagerArray.end(), (CImageManager*)hHandle);
		if (it != g_ImageManagerArray.end())
		{
			CImageManager* pImageManager = (CImageManager*)hHandle;
			
			int iImageSize = MojingTextureBakerAPI_GetBakerTextureSize(hHandle);
			// 1 建立临时Texture
			char * pScript  = NULL;
#ifndef WIN32
			unsigned int iTextureId = 0;
			glGenTextures(1, &iTextureId);
			glBindTexture(GL_TEXTURE_2D, iTextureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImageSize, iImageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			// 2 执行Baker
			pScript = MojingTextureBakerAPI_BakerTexture(hHandle, iTextureId, false);
#endif // !WIN32
			

			

			// 3 保存脚本
			if (pScript && *pScript)
			{
				FILE * pScriptFile = fopen(szScriptFileName, "w+");
				if (pScriptFile)
				{
					fputs(pScript, pScriptFile);
					fflush(pScriptFile);
					fclose(pScriptFile);
				}
				delete pScript;
			}
	

		}
	}

}
// 4 保存图片 最笨的办法，从显存里面把数据拷贝出来
void MojingTextureBakerAPI_SaveImage(int iTextureID, int iWidth , int iHeight, const char *szImageFileName)
{
	unsigned char *pPixelData = new unsigned char[iWidth * iHeight * 4];
	memset(pPixelData, 0x88, iWidth * iHeight * 4);
#ifndef _WIN32
	// 建立 FBO
	static unsigned int FBO = 0;
	if (FBO == 0 || ! glIsFramebuffer(FBO))
	{
		glGenFramebuffers(1, &FBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iTextureID, 0);
/*	// 选中使用的图片
	glBindTexture(GL_TEXTURE_2D, iTextureID);
//	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	*/
	// 读取数据
	// glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);
	glReadPixels(0, 0, iWidth, iHeight, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);
	
//	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CheckErrors("SaveImage");
	MojingTextureBakerAPI_SaveImageRGBA(pPixelData,iWidth , iHeight , szImageFileName);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#else
#endif
	// glDeleteFramebuffers(1, &FBO);
	delete pPixelData;

}
void MojingTextureBakerAPI_SaveImageRGBA(const unsigned char * pBuffer, int iWidth, int iHeight, const char *szImageFileName)
{
	unsigned char BitmapFileheader[] = {
		0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	int iFileSizeOffset = 2;// FILE SIZE
	int iWidthOffset = 0x12;
	int iHeightOffset = iWidthOffset + 4;
	
	int iDataSize = iWidth * iHeight * 4;
	int iFileDataSize = 0x36 + iDataSize;
	
	*((int *)(BitmapFileheader + iFileSizeOffset)) = iFileDataSize;
	*((int *)(BitmapFileheader + iWidthOffset)) = iWidth;
	*((int *)(BitmapFileheader + iHeightOffset)) = iHeight;


	FILE *pFile = fopen(szImageFileName, "wb+");
	if (pFile)
	{
		fwrite(BitmapFileheader , 1 , 0x36 , pFile );
		int iWrited = 0;
		while (iWrited < iDataSize)
		{
			int iThisWrite = (int)fmin(1024 * 512, iDataSize - iWrited);
			fwrite(pBuffer + iWrited, 1, iThisWrite, pFile);
			fflush(pFile);
			iWrited += iThisWrite;
		}
		fclose(pFile);
	}
}
#ifndef WIN32
const char * GL_ErrorForEnum(const GLenum e)
{
	switch (e)
	{
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	default: return "Unknown gl error code";
	}
}
bool GL_CheckErrors(const char * logTitle)
{
	bool hadError = false;

	// There can be multiple errors that need reporting.
	do
	{
		GLenum err = glGetError();
		if (err == GL_NO_ERROR)
		{
			break;
		}
		hadError = true;
		char szError[512];
		strcpy(szError, GL_ErrorForEnum(err));
		if (err == GL_OUT_OF_MEMORY)
		{
			//FAIL"GL_OUT_OF_MEMORY");
		}
	} while (1);
	return hadError;
}
#endif // WIN32
