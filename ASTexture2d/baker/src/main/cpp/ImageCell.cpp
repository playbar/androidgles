#include "ImageCell.h"
#include "ImageInfo.h"

CImageCell::CImageCell(unsigned int uiSize):
m_Size(uiSize, uiSize),
m_Type(ImageCellType_Empty),
m_PositionType(ImageCellPosition_LT),
m_pParent(NULL),
m_pSubImage(NULL),
m_pSubCellLT(NULL),
m_pSubCellRT(NULL),
m_pSubCellLB(NULL),
m_pSubCellRB(NULL)
{
}
CImageCell::CImageCell(CImageInfo *pSubImage):
m_Type(ImageCellType_Image),
m_Size(pSubImage->CalculateMinCellSize()),
m_PositionType(ImageCellPosition_LT),
m_pParent(NULL),
m_pSubImage(pSubImage),
m_pSubCellLT(NULL),
m_pSubCellRT(NULL),
m_pSubCellLB(NULL),
m_pSubCellRB(NULL)
{
	m_pSubImage->SetCell(this);
}

CImageCell::CImageCell(unsigned int uiWidth, unsigned int uiHeight):
m_Size(uiWidth, uiHeight),
m_Type(ImageCellType_Empty),
m_PositionType(ImageCellPosition_LT),
m_pParent(NULL),
m_pSubImage(NULL),
m_pSubCellLT(NULL),
m_pSubCellRT(NULL),
m_pSubCellLB(NULL),
m_pSubCellRB(NULL)
{

}
CImageCell::CImageCell(const CImageSize &Size):
m_Size(Size),
m_Type(ImageCellType_Empty),
m_PositionType(ImageCellPosition_LT),
m_pParent(NULL),
m_pSubImage(NULL),
m_pSubCellLT(NULL),
m_pSubCellRT(NULL),
m_pSubCellLB(NULL),
m_pSubCellRB(NULL)
{

}

CImageCell::CImageCell(CImageCell *pSubCellLT, CImageCell *pSubCellRT/* = NULL*/, CImageCell *pSubCellLB /*= NULL*/, CImageCell *pSubCellRB /*= NULL*/):
m_Size(pSubCellLT->GetSize().GetWidth() * 2, pSubCellLT->GetSize().GetHeight() * 2),
m_Type(ImageCellType_Cells),
m_PositionType(ImageCellPosition_LT),
m_pParent(NULL),
m_pSubImage(NULL),
m_pSubCellLT(pSubCellLT),
m_pSubCellRT(pSubCellRT),
m_pSubCellLB(pSubCellLB),
m_pSubCellRB(pSubCellRB)
{
	CImageSize SizeLT, SizeRT, SizeLB, SizeRB;
	if (m_pSubCellLT)
	{
		if (m_pSubCellLT->m_pParent)
		{// ERROR!!
			m_pSubCellLT->m_pParent = NULL;
		}
		m_pSubCellLT->m_pParent = this;
		m_pSubCellLT->m_PositionType = ImageCellPosition_LT;
	}

	if (m_pSubCellRT)
	{
		if (m_pSubCellRT->m_pParent)
		{// ERROR!!
			m_pSubCellRT->m_pParent = NULL;
		}
		m_pSubCellRT->m_pParent = this;
		m_pSubCellRT->m_PositionType = ImageCellPosition_RT;
	}

	if (m_pSubCellLB)
	{
		if (m_pSubCellLB->m_pParent)
		{// ERROR!!
			m_pSubCellLB->m_pParent = NULL;
		}
		m_pSubCellLB->m_pParent = this;
		m_pSubCellLB->m_PositionType = ImageCellPosition_LB;
	}

	if (m_pSubCellRB)
	{
		if (m_pSubCellRB->m_pParent)
		{// ERROR!!
			m_pSubCellRB->m_pParent = NULL;
		}
		m_pSubCellRB->m_pParent = this;
		m_pSubCellRB->m_PositionType = ImageCellPosition_RB;
	}
	
}

CImageCell::~CImageCell()
{
	if (m_pSubCellLT)
	{
		delete m_pSubCellLT;
	}

	if (m_pSubCellRT)
	{
		delete m_pSubCellRT;
	}

	if (m_pSubCellLB)
	{
		delete m_pSubCellLB;
	}

	if (m_pSubCellRB)
	{
		delete m_pSubCellRB;
	}
	m_pSubCellLT = m_pSubCellRT = m_pSubCellLB = m_pSubCellRB = NULL;
}

int CImageCell::GetCellsCount()const
{
	int iRet = 1;
	if (m_pSubCellLT)
		iRet += m_pSubCellLT->GetCellsCount();
	if (m_pSubCellRT)
		iRet += m_pSubCellRT->GetCellsCount();
	if (m_pSubCellLB)
		iRet += m_pSubCellLB->GetCellsCount();
	if (m_pSubCellRB)
		iRet += m_pSubCellRB->GetCellsCount();
	return iRet;
}

CImageCell* CImageCell::MakeToSquareCell()
{
	if (IsSquareCell() && m_Size == m_pSubImage->CalculateCellSize())
		return NULL;
	if (m_pParent)
	{// ERROR!!!
		delete m_pParent;
	}
	CImageSize CellSize = m_pSubImage->CalculateCellSize();
	int iParentSize = (int)fmax(CellSize.GetWidth(), CellSize.GetHeight());
	int iCellSize = (int)fmin(m_Size.GetWidth(), m_Size.GetHeight());
	bool bLeftToRight = m_Size.GetWidth() < m_Size.GetHeight();
	CImageCell* pRet = new CImageCell(iParentSize);
	int iFirstPart = bLeftToRight ? m_Size.GetHeight() : m_Size.GetWidth();
	if (iFirstPart != iParentSize)
	{
		pRet->Split(iFirstPart, !bLeftToRight , 1);
		pRet->GetSubCellLT()->MargeIn(this, bLeftToRight, 1);
	}
	else
	{
		pRet->MargeIn(this, bLeftToRight, 1);
	}
	
	return pRet;
}

int CImageCell::PrintCells(char * szPrintBegin)const
{
	int iRet = 0;

	if (m_pSubImage)
	{
		CPosition Pt = GetPosition();
		CImageSize Sz(m_Size);
		sprintf(szPrintBegin, "%d,%d,%d,%d,___<--%dx%d\n", Pt.GetX(), Pt.GetY(), Sz.GetWidth(), Sz.GetHeight(), m_Size.GetWidth(), m_Size.GetHeight());
		iRet += strlen(szPrintBegin);
	}

	if (m_Type == ImageCellType_Empty)
	{
		CPosition Pt = GetPosition();
		CImageSize Sz(m_Size);
		iRet = sprintf(szPrintBegin, "%d,%d,%d,%d,EMPTY\n", Pt.GetX(), Pt.GetY(), Sz.GetWidth(), Sz.GetHeight(), m_Size.GetWidth(), m_Size.GetHeight());
	}

	if (m_pSubCellLT)
		iRet += m_pSubCellLT->PrintCells(szPrintBegin + iRet);
	if (m_pSubCellRT)
		iRet += m_pSubCellRT->PrintCells(szPrintBegin + iRet);
	if (m_pSubCellLB)
		iRet += m_pSubCellLB->PrintCells(szPrintBegin + iRet);
	if (m_pSubCellRB)
		iRet += m_pSubCellRB->PrintCells(szPrintBegin + iRet);
	return iRet;
}

void CImageCell::UpdatePosition(CPosition ptNewPosition)
{
	m_Position = ptNewPosition;
	CImageSize SizeLT;
	if (m_pSubCellLT)
	{
		SizeLT = m_pSubCellLT->GetSize();
		m_pSubCellLT->UpdatePosition(ptNewPosition);
	}
	if (m_pSubCellRT)
		m_pSubCellRT->UpdatePosition(CPosition(ptNewPosition.GetX() + SizeLT.GetWidth(), ptNewPosition.GetY()));
	if (m_pSubCellLB)
		m_pSubCellLB->UpdatePosition(CPosition(ptNewPosition.GetX(), ptNewPosition.GetY() + SizeLT.GetHeight()));
	if (m_pSubCellRB)
		m_pSubCellRB->UpdatePosition(CPosition(ptNewPosition.GetX() + SizeLT.GetWidth(), ptNewPosition.GetY() + SizeLT.GetHeight()));
}
void CImageCell::UpdatePosition()
{
	CImageCell *pParent = this;
	while (pParent->GetParent())
	{
		pParent = pParent->GetParent();
	}
	pParent->UpdatePosition(CPosition(0, 0));
}
void CImageCell::RemoveSmallEmptyCell(CImageCell * &pSubCell, const unsigned int iMinSize)
{
	return;
	if (pSubCell)
	{
		if (pSubCell->m_Type == ImageCellType_Empty)
		{
			if (!pSubCell->GetSize().TestBiggerThen(iMinSize))
			{
				delete pSubCell;
				pSubCell = NULL;
			}
		}
		else if (pSubCell ->m_Type == ImageCellType_Cells)
		{
			pSubCell->RemoveSmallEmptyCell(iMinSize);
		}
	}
}
void CImageCell::RemoveSmallEmptyCell(const unsigned int iMinSize)
{
	return;
	RemoveSmallEmptyCell(m_pSubCellLT, iMinSize); 
	RemoveSmallEmptyCell(m_pSubCellRT, iMinSize); 
	RemoveSmallEmptyCell(m_pSubCellLB, iMinSize); 
	RemoveSmallEmptyCell(m_pSubCellRB, iMinSize); 
}
int CImageCell::GetEmptySubCellCount()const
{
	int iRet = 0;
	if (m_Type == ImageCellType_Empty&&
		m_pSubCellLT == NULL &&
		m_pSubCellRT == NULL &&
		m_pSubCellLB == NULL &&
		m_pSubCellRB == NULL)
	{
		iRet++;
	}
	else
	{
		if (m_pSubCellLT)
			iRet += m_pSubCellLT->GetEmptySubCellCount();
		if (m_pSubCellRT)
			iRet += m_pSubCellRT->GetEmptySubCellCount();
		if (m_pSubCellLB)
			iRet += m_pSubCellLB->GetEmptySubCellCount();
		if (m_pSubCellRB)
			iRet += m_pSubCellRB->GetEmptySubCellCount();
	}
	return iRet;
}

int CImageCell::GetEmptySubCells(CImageCell **pOutBuffer, int iNextIndex)
{
	int iRet = 0;
	if (m_Type == ImageCellType_Empty)
	{
		iRet++;
		if (pOutBuffer)
			pOutBuffer[iNextIndex] = this;
	}

	if (m_pSubCellLT)
		iRet += m_pSubCellLT->GetEmptySubCells(pOutBuffer, iNextIndex + iRet);
	if (m_pSubCellRT)
		iRet += m_pSubCellRT->GetEmptySubCells(pOutBuffer, iNextIndex + iRet);
	if (m_pSubCellLB)
		iRet += m_pSubCellLB->GetEmptySubCells(pOutBuffer, iNextIndex + iRet);
	if (m_pSubCellRB)
		iRet += m_pSubCellRB->GetEmptySubCells(pOutBuffer, iNextIndex + iRet);
	return iRet;
}

float CImageCell::TestMargeCellsRate(CImageCell **pMargeBegin, int iCount)const
{
	float fRet = 0;
	if (m_Type == ImageCellType_Empty)
	{
		float fThisProportion = (float)m_Size.GetProportion();
		CImageCell * pThisTry = pMargeBegin[0];
		CImageSize TryImageSize = pThisTry->GetSize();
		int iMaxRow = m_Size.GetHeight() / TryImageSize.GetHeight();
		int iMaxCol = m_Size.GetHeight() / TryImageSize.GetHeight();
		int iMaxMargeCount = (int)fmin(iCount, iMaxRow * iMaxCol);
		if (iMaxRow < 2 || iMaxCol < 2)
		{
			float fSubCellProportion = (float)(TryImageSize.GetProportion() * iMaxMargeCount);
			fRet = fSubCellProportion / fThisProportion;
		}
	}
	return fRet;
}

int CImageCell::TestMargeCellsCount(CImageCell **pMargeBegin, int iCount)const
{
	int iRet = 0;
	if (m_Type == ImageCellType_Empty)
	{
		CImageCell * pThisTry = pMargeBegin[0];
		CImageSize TryImageSize = pThisTry->GetSize();
		int iMaxRow = m_Size.GetHeight() / TryImageSize.GetHeight();
		int iMaxCol = m_Size.GetWidth() / TryImageSize.GetWidth();
		int iMaxMargeCount = (int)fmin(iCount, iMaxRow * iMaxCol);
		if (iMaxRow < 2 || iMaxCol < 2)
		{
			iRet = iMaxMargeCount;
		}
	}
	return iRet;
}

int CImageCell::MargeCells(CImageCell **pMargeBegin , int iCount)
{
	int iRet = 0;
	if (m_Type == ImageCellType_Empty&&
		m_pSubCellLT == NULL &&
		m_pSubCellRT == NULL &&
		m_pSubCellLB == NULL &&
		m_pSubCellRB == NULL)
	{
		CImageCell * pThisTry = pMargeBegin[0];
		CImageSize TryImageSize = pThisTry->GetSize();
		int iMaxRow = m_Size.GetHeight() / TryImageSize.GetHeight();
		int iMaxCol = m_Size.GetWidth() / TryImageSize.GetWidth();
		int iMiniEdge = (int)fmin(TryImageSize.GetWidth() , TryImageSize.GetHeight());
		
		bool bLeftToRight = iMaxCol > 1;
		if (iMaxRow < 2 || iMaxCol < 2)
		{
			int iMaxMargeCount = (int)fmin(iCount, iMaxRow * iMaxCol);
			int iMargeEdge;
			if (bLeftToRight)
			{				
				iMargeEdge = TryImageSize.GetHeight();
			}
			else
			{
				iMargeEdge = TryImageSize.GetWidth();
			}
			// 先把自己分成两份：一份(LT)用于接收合并的格子，另一份作为空白待用
			Split(iMargeEdge, !bLeftToRight, 1);
			
			CImageCell *pNextMargeInto = m_pSubCellLT;
			
			while (iRet < iMaxMargeCount)
			{
				pNextMargeInto = pNextMargeInto->MargeIn(pMargeBegin[iRet++], bLeftToRight, 1);
				// pNextMargeInto = pNextMargeInto->m_pSubCellLT;
			}
		}

	}
	return iRet;
}

int CImageCell::Split(const unsigned int &iSize, const bool &bLeftToRight, const unsigned int iSizeLimit)
{
	int iRet = 0;
	if (m_Type == ImageCellType_Empty&&
		m_pSubCellLT == NULL &&
		m_pSubCellRT == NULL &&
		m_pSubCellLB == NULL &&
		m_pSubCellRB == NULL)
	{
		m_Type = ImageCellType_Cells;
		CImageSize SizeA = m_Size;
		CImageSize SizeB = SizeA.Split(iSize, bLeftToRight);
		if (bLeftToRight)
		{
			SizeA.SetWidth(iSize);
		}
		else
		{
			SizeA.SetHeight(iSize);
		}
		m_pSubCellLT = new CImageCell(SizeA);
		m_pSubCellLT->m_pParent = this;
		m_pSubCellLT->m_PositionType = ImageCellPosition_LT;
		iRet++;
		if (SizeB.TestBiggerThen(iSizeLimit))
		{
			if (bLeftToRight)
			{
				m_pSubCellRT =  new CImageCell(SizeB);
				m_pSubCellRT->m_PositionType = ImageCellPosition_RT;
				m_pSubCellRT->m_pParent = this;
				if (m_pSubCellLT->m_Size.GetWidth() + m_pSubCellRT->m_Size.GetWidth() > m_Size.GetWidth())
				{// ERROR
					delete m_pSubCellRT;
				}
			}
			else
			{
				m_pSubCellLB =  new CImageCell(SizeB);
				m_pSubCellLB->m_PositionType = ImageCellPosition_LB;	
				m_pSubCellLB->m_pParent = this;
				if (m_pSubCellLT->m_Size.GetHeight() + m_pSubCellLB->m_Size.GetHeight() > m_Size.GetHeight())
				{// ERROR
					delete m_pSubCellLB;
				}
			}
			
			iRet++;
		}
	}
	return iRet;		
}

CImageCell * CImageCell::MargeIn(CImageCell *pSubCell, bool bLeftToRight, int iSizeLimit )
{
	CImageCell * pRet = NULL;
	if (m_Type == ImageCellType_Empty&&
		m_pSubCellLT == NULL &&
		m_pSubCellRT == NULL &&
		m_pSubCellLB == NULL &&
		m_pSubCellRB == NULL)
	{
		m_Type = ImageCellType_Cells;
		m_pSubCellLT = pSubCell;
		if (m_pSubCellLT->m_pParent)
		{// ERROR
			delete m_pSubCellLT->m_pParent;
		}
		m_pSubCellLT->m_pParent = this;
		m_pSubCellLT->m_PositionType = ImageCellPosition_LT; 
	
		CImageSize EmptyCellSizeA;
		if (bLeftToRight)
		{
			EmptyCellSizeA = m_Size.Split(m_pSubCellLT->m_Size.GetWidth(), bLeftToRight);
		}
		else
		{
			EmptyCellSizeA = m_Size.Split(m_pSubCellLT->m_Size.GetHeight(), bLeftToRight);
		}
		
		if (EmptyCellSizeA.TestBiggerThen(iSizeLimit))
		{
			if (bLeftToRight)
			{
				pRet = m_pSubCellRT = new CImageCell(EmptyCellSizeA);
				m_pSubCellRT->m_pParent = this;
				m_pSubCellRT->m_PositionType = ImageCellPosition_RT;
				if (m_pSubCellLT->m_Size.GetWidth() + m_pSubCellRT->m_Size.GetWidth() > m_Size.GetWidth())
				{// ERROR
					delete pRet;
				}
			}
			else
			{
				pRet = m_pSubCellLB =  new CImageCell(EmptyCellSizeA);
				m_pSubCellLB->m_pParent = this;
				m_pSubCellLB->m_PositionType = ImageCellPosition_LB;		

				if (m_pSubCellLT->m_Size.GetHeight() + m_pSubCellLB->m_Size.GetHeight() > m_Size.GetHeight())
				{// ERROR
					delete pRet;
				}
			}
		}// end of if (SubCellSize.TestBiggerThen(iSizeLimit))

		// check error
		
	}
	else
	{
		// ERROR
		pRet = NULL;
	}
	return pRet;
}