#pragma once
#include "ImageSize.h"
class CImageCell;
#ifdef WIN32
#include <string>
using namespace std;
string&   replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value);
#endif


class CImageInfo
{
public:
	CImageInfo(unsigned int uiWidth = 0, unsigned int uiHeight = 0);
	CImageInfo(unsigned int uiX, unsigned int uiY, unsigned int uiWidth, unsigned int uiHeight, TEXTURE_ID iTextureID, int index);
	virtual ~CImageInfo();
	CImageSize CalculateCellSize()const;
	CImageSize CalculateMinCellSize()const;
	int GetMaxEdge()const;
	CPosition GetPosition()const;
	void UpdateBakedPosition(TEXTURE_ID iBakedTextureID);
	int PrintScript(char * szOutBuffer) const;
#ifndef WIN32
	void DoBaker(int iTargerTextureID , int iCurrentSourceTexture)const;
#else
	void DoBaker(Graphics* pGraphics)const;
#endif
	CLASS_MEMBER(CPosition, m_, SrcPosition);
	CLASS_MEMBER(CImageSize , m_ , Size);
	CLASS_MEMBER(CImageCell*, m_p, Cell);

	CLASS_MEMBER(TEXTURE_ID, m_i, TextureID);
	CLASS_MEMBER(int, m_i, Index);

	bool operator == (const CImageInfo & Other)const{
		return m_SrcPosition == Other.m_SrcPosition &&
			m_Size == Other.m_Size &&
			m_iTextureID == Other.m_iTextureID;
	};
};


class CImageInfoArray
{
	int FindInsertPosition(int iCellSize)const
	{
		int iPos = 0;
		while (iPos < m_iSize &&
			m_pImageCellBuffer[iPos]->GetMaxEdge() > iCellSize)
		{
			iPos++;
		}
		return iPos;
	};
	int ReSize()
	{
		CImageInfo** pNewBuffer = new CImageInfo*[m_iRealloc + m_iMaxSize];
		
		memset(pNewBuffer, 0, sizeof(CImageInfo*) * (m_iMaxSize + m_iRealloc));
		memcpy(pNewBuffer, m_pImageCellBuffer, sizeof(CImageInfo*)* m_iMaxSize);
		m_iMaxSize += m_iRealloc;
		delete m_pImageCellBuffer;
		m_pImageCellBuffer = pNewBuffer;
		return m_iMaxSize;
	};
	int Insert(int iPos, CImageInfo* pNewImage)
	{
		if (m_iSize + 1 > m_iMaxSize)
			ReSize();
		if (iPos != m_iSize)
		{
			memmove(m_pImageCellBuffer + iPos + 1, m_pImageCellBuffer + iPos, (m_iSize - iPos) *  sizeof(CImageInfo*));
		}
		m_pImageCellBuffer[iPos] = pNewImage;
		return ++m_iSize;
	};
public:
	CImageInfoArray() :
		m_iSize(0), m_iRealloc(64), m_iMaxSize(0), m_pImageCellBuffer(NULL)
	{
	};
	~CImageInfoArray()
	{
		if (m_pImageCellBuffer)
		{
			for (int i = 0; i < m_iSize; i++)
			{
				delete m_pImageCellBuffer[i];
			}
			delete m_pImageCellBuffer;
		}
	}
	// 返回插入的位置
	int AddImage(CImageInfo* pNewImage)
	{
		int iPos = FindInsertPosition(pNewImage->GetMaxEdge());
		Insert(iPos , pNewImage);
		return iPos;
	}

	// 返回剩余的大小
	int RemoveImage(int iPos)
	{
		if (iPos >= 0 && iPos < m_iSize)
		{
			delete m_pImageCellBuffer[iPos];
			memmove(m_pImageCellBuffer + iPos, m_pImageCellBuffer + iPos + 1, (m_iSize - iPos) *  sizeof(CImageInfo*));
			--m_iSize;
			m_pImageCellBuffer[m_iSize] = NULL;
		}
		return m_iSize;
	}
	int Find(const CImageInfo* pNewImage)const
	{
		int iRet = -1;
		for (int iIndex = 0; iIndex < m_iSize; iIndex++)
		{
			if (*(m_pImageCellBuffer[iIndex]) == *pNewImage)
			{
				iRet = iIndex;
				break;
			}
		}
		return iRet;
	}
	int Find(TEXTURE_ID TextureID,int index, unsigned int uiX, unsigned int uiY, unsigned int uiWidth, unsigned int uiHeight)const
	{
		CImageInfo Image(uiX, uiY, uiWidth, uiHeight, TextureID, index);
		int iRet = -1;
		for (int iIndex = 0; iIndex < m_iSize; iIndex++)
		{
			if (*(m_pImageCellBuffer[iIndex]) == Image)
			{
				iRet = iIndex;
				break;
			}
		}
		return iRet;
	}
	int Find(TEXTURE_ID iTextureID)
	{
		int iRet = -1;
		for (int iIndex = 0; iIndex < m_iSize; iIndex++)
		{
			if ((m_pImageCellBuffer[iIndex])->GetTextureID() == iTextureID)
			{
				iRet = iIndex;
				break;
			}
		}
		return iRet;
	}
	CImageInfo*& operator [](int iIndex)const{ return m_pImageCellBuffer[iIndex]; };
	CLASS_INTERFACE(int, m_i, Size);
	CLASS_INTERFACE(int, m_i, MaxSize);
	CLASS_INTERFACE(int, m_i, Realloc);
	CLASS_INTERFACE(CImageInfo**, m_p, ImageCellBuffer);

};
