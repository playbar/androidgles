#include "ImageSize.h"


CImageSize::CImageSize()
{
}

CImageSize::CImageSize(unsigned int uiWidth, unsigned int uiHeight):
m_uiWidth(uiWidth),
m_uiHeight(uiHeight)
{
	
}

CImageSize::~CImageSize()
{
}

unsigned long CImageSize::GetProportion()const
{
	return ((unsigned long)m_uiWidth) * m_uiHeight;
}