#pragma once
#include "Base.h"
HANDLE MojingTextureBakerAPI_BeginBaker(int iMaxSize);
int MojingTextureBakerAPI_AddTexture(HANDLE hHandle, TEXTURE_ID iTextureID, int index, int iX, int iY, int iWidth, int iHeight);
int MojingTextureBakerAPI_RemoveTexture(HANDLE hHandle, TEXTURE_ID iTextureID, int index, int iX, int iY, int iWidth, int iHeight);
int MojingTextureBakerAPI_GetBakerTextureSize(HANDLE hHandle, FP_UpdateProgressInfo pUpdateProgressInfo = NULL);
char * MojingTextureBakerAPI_BakerTexture(HANDLE hHandle, TEXTURE_ID iTextureID, bool bReplaceByBakedTexture, FP_UpdateProgressInfo pUpdateProgressInfo = NULL);
char * MojingTextureBakerAPI_PrintRects(HANDLE hHandle);
char * MojingTextureBakerAPI_PrintCells(HANDLE hHandle);
char * MojingTextureBakerAPI_PrintJson(HANDLE hHandle, TEXTURE_ID iTextureID);

void MojingTextureBakerAPI_EndBaker(HANDLE hHandle);

void MojingTextureBakerAPI_SaveToFile(HANDLE hHandle, const char *szScriptFileName, const char *szImageFileName);
void MojingTextureBakerAPI_SaveImage(int iTextureID, int iWidth, int iHeight, const char *szImageFileName);
void MojingTextureBakerAPI_SaveImageRGBA(const unsigned char * pBuffer, int iWidth, int iHeight, const char *szImageFileName);
/*560 - 107*/
