#include "ImageManager.h"


CImageManager::CImageManager():
m_pRoot(NULL), m_iMaxSize(0)
{
}


CImageManager::~CImageManager()
{
	if (m_pRoot)
		delete m_pRoot;
}
void SortCells(CImageCell **pCells, int iCount)
{
	
	for (int i = 0; i < iCount - 1; i++)
	{
		int iMaxSize = pCells[i]->GetSize().GetProportion(), iMaxIndex = i;
		for (int j = i + 1; j < iCount - 1; j++)
		{
			if (iMaxSize < (int)pCells[j]->GetSize().GetProportion())
			{
				iMaxSize = pCells[j]->GetSize().GetProportion();
				iMaxIndex = j;
			}
		}
		if (iMaxIndex != i)
		{
			CImageCell * pCell = pCells[i];
			pCells[i] = pCells[iMaxIndex];
			pCells[iMaxIndex] = pCell;
		}
	}
}
int CImageManager::CalculationImagePosition(FP_UpdateProgressInfo fpUpdateProgressInfo)
{
	if (m_pRoot)
		delete m_pRoot;

	
	m_pRoot = NULL;
	int iBufferSize = (m_ImageArray.GetSize() / 4 + 3) * 4 ;
	
	CImageCell** pCellList = new CImageCell*[iBufferSize];
	memset(pCellList, 0, sizeof(CImageCell*)* iBufferSize);

	int iCount = m_ImageArray.GetSize();
	int iCellCount = 0;
	int iIndex = 0;
	int iEmptyCellIndex = 0;
	ProgressInfo PI;
	if (fpUpdateProgressInfo)
	{
		sprintf(PI.m_szProgressText , "准备合批前动作....");
		PI.m_iProgressNow = 0;
		PI.m_iProgressMax = iCount;
		fpUpdateProgressInfo(&PI);
	}

	// 1 制作容器
	for (iIndex = 0; iIndex < iCount; iIndex++)
	{
		if (fpUpdateProgressInfo)
		{
			PI.m_iProgressNow = iIndex + 1;
			fpUpdateProgressInfo(&PI);
		}
		CImageCell *pNewImage = new CImageCell(m_ImageArray[iIndex]);
		if (pNewImage->IsSquareCell() && pNewImage->GetSize() == m_ImageArray[iIndex]->CalculateCellSize())
		{
			pCellList[iCellCount++] = pNewImage;
		}
		else
		{
			bool bMarged = 0;
			int iEmptyCellCount = 0;
			for (int i = iCellCount - 1; i >= 0; i--)
			{
				iEmptyCellCount += pCellList[i]->GetEmptySubCellCount();
			}
			if (iEmptyCellCount > 0)
			{
				CImageCell **pEmptyCellList = new CImageCell*[iEmptyCellCount];
				memset(pEmptyCellList, 0, sizeof(CImageCell*)* iEmptyCellCount);

				int iNextEmptyCell = 0;
				for (int i = iCellCount - 1; i >= 0; i--)
				{
					iNextEmptyCell += pCellList[i]->GetEmptySubCells(pEmptyCellList, iNextEmptyCell);
				}
				for (int i = iNextEmptyCell - 1; i >= 0; i--)
				{
					if (pEmptyCellList[i]->TestMargeCellsCount(&pNewImage, 1))
					{
						pEmptyCellList[i]->MargeCells(&pNewImage, 1);
						bMarged = true;
						break;
					}
				}
				delete pEmptyCellList;
			}
			if (!bMarged)
				pCellList[iCellCount++] = pNewImage->MakeToSquareCell();
		}
	}
	// 2 合并
	if (fpUpdateProgressInfo)
	{
		sprintf(PI.m_szProgressText, "计算合批摆放方式....");
		PI.m_iProgressNow = 0;
		PI.m_iProgressMax = iCellCount;
		fpUpdateProgressInfo(&PI);
	}

	int iOrder = 0;
	while (iCellCount > 1)
	{
		if (fpUpdateProgressInfo)
		{
			PI.m_iProgressNow = PI.m_iProgressMax - iCellCount;
			fpUpdateProgressInfo(&PI);
		}
		int iMargeEnd = iCellCount - 1;
		int iMargeBegin = iMargeEnd;
		int iCurrentSize = pCellList[iMargeEnd]->GetSize().GetWidth();
		// 2.1 寻找本轮的合并点
		while (iMargeBegin - 1 >= 0 && pCellList[iMargeBegin - 1]->GetSize().GetWidth() == iCurrentSize)
		{
			iMargeBegin--;
		}

		int iThisMargeCount = iMargeEnd - iMargeBegin + 1;
		// 2.2 尝试插空挡
		int iMargeBeginOld = iMargeBegin;
		if (iMargeBegin >= 1)// 如果是0的话，表示全部剩余节点需要合并。就没有意义了。
		{
			CImageCell** pEmptyCellList = NULL;
			int iEmptyCellCount = 0;
			iEmptyCellIndex = 0;
			
			for (int iEmptyScan = 0; iEmptyScan < iMargeBegin; iEmptyScan++)
			{
				pCellList[iEmptyScan]->RemoveSmallEmptyCell(iCurrentSize);
				iEmptyCellCount += pCellList[iEmptyScan]->GetEmptySubCellCount();
			}
			if (iEmptyCellCount)
			{
				pEmptyCellList = new CImageCell*[iEmptyCellCount + 16];
				memset(pEmptyCellList, 0, sizeof(CImageCell*)* (iEmptyCellCount + 16));
				for (int iEmptyScan = 0; iEmptyScan < iMargeBegin; iEmptyScan++)
				{
					iEmptyCellIndex += pCellList[iEmptyScan]->GetEmptySubCells(pEmptyCellList, iEmptyCellIndex);
				}
				
				SortCells(pEmptyCellList, iEmptyCellIndex);
				
				for (int iEmptyIndex = iEmptyCellIndex - 1; iEmptyIndex >= 0 && (iMargeEnd - iMargeBegin + 1 > 0); iEmptyIndex--)
				{
					if (pEmptyCellList[iEmptyIndex]->GetSubCellLT() || pEmptyCellList[iEmptyIndex]->GetSubCellLB() ||
						pEmptyCellList[iEmptyIndex]->GetSubCellRT() || pEmptyCellList[iEmptyIndex]->GetSubCellRB())
					{// ERROR
						delete pEmptyCellList[iEmptyIndex];
					}
					CImageCell *pMargeTo = pEmptyCellList[iEmptyIndex];
					int iThisMargeCount = pMargeTo->TestMargeCellsCount(pCellList + iMargeBegin, iMargeEnd - iMargeBegin + 1);
					float fThisMargeRate = pMargeTo->TestMargeCellsRate(pCellList + iMargeBegin, iMargeEnd - iMargeBegin + 1);
					if (iThisMargeCount > 0 &&(fThisMargeRate > 0.1 || iMargeBegin == 1 || iThisMargeCount == iMargeEnd - iMargeBegin + 1))
					{
						
						int iMargeCount = pMargeTo->MargeCells(pCellList + iMargeBegin, iMargeEnd - iMargeBegin + 1);
						//memcpy(pCellList + iMargeBegin, pCellList + iMargeBegin + iMargeCount, sizeof(CImageCell*)* (iMargeEnd - iMargeBegin + 1 - iMargeCount));
						memset(pCellList + iMargeBegin , 0, iMargeCount * sizeof(CImageCell*));
						iMargeBegin += iMargeCount;
						pEmptyCellList[iEmptyIndex] = NULL;
					}
				}
				delete pEmptyCellList;
			}
			if (iMargeBeginOld < iMargeBegin)
			{
				int iLeft = iMargeEnd - iMargeBegin + 1;
				int iMarged = iMargeBegin - iMargeBeginOld;
				if (iLeft > 0)
				{	
					memcpy(pCellList + iMargeBeginOld, pCellList + iMargeBegin, (iMargeEnd - iMargeBegin + 1) * sizeof(CImageCell*));
					
					iMargeBegin = iMargeBeginOld;
					iMargeEnd = iMargeBegin + iLeft-1;
					memset(pCellList + iMargeEnd + 1, 0, iMarged*sizeof(CImageCell*));
				}
				else
				{
					iCellCount = iMargeBeginOld;
					continue;// 没有需要继续处理的节点了
				}
				
			}
		}
		if (iMargeEnd - iMargeBegin + 1 > 0)
		{
			iOrder = !iOrder;
			int iInsertPosition = iMargeBegin;
			while (pCellList[iMargeBegin])
			{
				CImageCell* pNewCell = new CImageCell(pCellList[iMargeBegin], pCellList[iMargeBegin + 1 + iOrder], pCellList[iMargeBegin + 1 + !iOrder], pCellList[iMargeBegin + 3]);
				memset(pCellList + iMargeBegin, 0, sizeof(CImageCell*)* 4);
				iMargeBegin += 4;
				pCellList[iInsertPosition++] = pNewCell;
			}
			iCellCount = iInsertPosition;
		}
		else
		{
			iCellCount = iMargeEnd + 1;
		}
	}

	m_pRoot = pCellList[0];
	delete [] pCellList;
	m_pRoot->UpdatePosition();

	return m_pRoot->GetSize().GetWidth();
}

char *CImageManager::PrintRects()const
{
	int iImageCount = m_ImageArray.GetSize();
	int iBufferSize = iImageCount * 32;
	char *pRet = new char[iBufferSize + 16];
	*pRet = 0;
	char *pPrintBegin = pRet;
	for (int iIndex = 0; iIndex < iImageCount; iIndex++)
	{
		
		CPosition Pt = m_ImageArray[iIndex]->GetPosition();
		CImageSize Sz = m_ImageArray[iIndex]->GetSize();
#ifndef WIN32
		const char * FMT = "\n%d,%d,%d,%d,%d";
		if (iIndex)
			pPrintBegin += sprintf(pPrintBegin, FMT, Pt.GetX(), Pt.GetY(), Sz.GetWidth(), Sz.GetHeight(), m_ImageArray[iIndex]->GetTextureID());
		else
			pPrintBegin += sprintf(pPrintBegin, FMT + 1, Pt.GetX(), Pt.GetY(), Sz.GetWidth(), Sz.GetHeight(), m_ImageArray[iIndex]->GetTextureID());
#else
		const char * FMT = "\n%d,%d,%d,%d,%s";
		if (iIndex)
			pPrintBegin += sprintf(pPrintBegin, FMT, Pt.GetX(), Pt.GetY(), Sz.GetWidth(), Sz.GetHeight(), m_ImageArray[iIndex]->GetTextureID().c_str());
		else
			pPrintBegin += sprintf(pPrintBegin, FMT + 1, Pt.GetX(), Pt.GetY(), Sz.GetWidth(), Sz.GetHeight(), m_ImageArray[iIndex]->GetTextureID().c_str());
#endif // !WIN32

		
	}
	return pRet;
}

char *CImageManager::PrintCells()const
{
	char *pRet = NULL;
	if (m_pRoot)
	{
		int iSize = m_pRoot->GetCellsCount();
		pRet = new char[iSize * 64];
		*pRet = 0;
		m_pRoot->PrintCells(pRet);
	}
	return pRet;
}