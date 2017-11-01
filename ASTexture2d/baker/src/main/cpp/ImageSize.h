#pragma once
#include "Base.h"

class CImageSize
{
public:
	CImageSize();
	CImageSize(unsigned int uiWidth, unsigned int uiHeight);
	virtual ~CImageSize();
	unsigned long GetProportion()const;
	bool operator == (const CImageSize & Other)const{
		return m_uiWidth == Other.m_uiWidth && m_uiHeight == Other.m_uiHeight;
	}
	bool TestBiggerThen(const unsigned int & iLimit)const
	{
		return m_uiWidth >= iLimit && m_uiHeight >= iLimit;
	}
	CImageSize Alignment(const unsigned int &iAlignment)const
	{
		int iW = (m_uiWidth + iAlignment  - 1)/ iAlignment * iAlignment;
		int iH = (m_uiHeight + iAlignment - 1) / iAlignment * iAlignment;
		return CImageSize(iW , iH);
	}
	CImageSize Split(const unsigned int & iSize, const bool & bLeftToRight)const
	{
		CImageSize Ret = *this;
		if (bLeftToRight)
		{
			Ret.m_uiWidth -= iSize;
		}
		else
		{
			Ret.m_uiHeight -= iSize;
		}
		return Ret;
	}
private:
	CLASS_MEMBER(unsigned int, m_ui , Width);
	CLASS_MEMBER(unsigned int, m_ui , Height);
};

class CPosition
{
public:
	CPosition():m_uiX(0),m_uiY(0){};
	CPosition(unsigned int uiX, unsigned int uiY) :m_uiX(uiX), m_uiY(uiY){};
	virtual ~CPosition(){};
	bool operator == (const CPosition & Other)const{
		return m_uiX == Other.m_uiX && m_uiY == Other.m_uiY;
	}
private:
	CLASS_MEMBER(unsigned int, m_ui, X);
	CLASS_MEMBER(unsigned int, m_ui, Y);
};