#pragma once
#include "ImageCell.h"
#include "ImageInfo.h"

class CImageManager
{
	
public:
	int CalculationImagePosition(FP_UpdateProgressInfo pUpdateProgressInfo = NULL);
	CImageManager();
	virtual ~CImageManager();
	CImageInfoArray m_ImageArray;
	char *PrintRects()const;
	char *PrintCells()const;
	CLASS_INTERFACE(CImageCell *, m_p, Root);
	CLASS_MEMBER(unsigned int , m_i, MaxSize);
};

