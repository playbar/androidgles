#pragma once
#include "Base.h"
#include "ImageSize.h"
enum ImageCellType
{
	ImageCellType_Image,
	ImageCellType_Empty, 
	ImageCellType_Cells
};
enum ImageCellPosition
{
	ImageCellPosition_LT,
	ImageCellPosition_RT,
	ImageCellPosition_LB,
	ImageCellPosition_RB
};

class CImageInfo;
class CImageCell
{
	// 通过父节点传入的位置，逐级递归更新所有的子节点
	void UpdatePosition(CPosition ptNewPosition);
	// 拆分单元格。
	int Split(const unsigned int &iSize, const bool &bLeftToRight, const unsigned int iSizeLimit = 16);
	// 把一个小的单元格合并到当前单元格里面。
	CImageCell * MargeIn(CImageCell *pSubCell , bool bLeftToRight , int iSizeLimit = 16);
	// 把本节点和所有子节点中，过小的单元格删除掉，节省一些空间
	void RemoveSmallEmptyCell(CImageCell * &pSubCell, const unsigned int iMinSize);
public:
	CImageCell(unsigned int uiSize);
	CImageCell(unsigned int uiWidth, unsigned int uiHeight);
	CImageCell(const CImageSize & Size);
	CImageCell(CImageInfo *pSubImage);
	CImageCell(CImageCell *pSubCellLT, CImageCell *pSubCellRT = NULL, CImageCell *pSubCellLB = NULL, CImageCell *pSubCellRB = NULL);
	virtual ~CImageCell();
	int GetCellsCount()const;
	int PrintCells(char * szPrintBegin)const;
	bool IsSquareCell()const{ return m_Size.GetWidth() == m_Size.GetHeight(); };
	CImageCell* MakeToSquareCell();	
	// 删除过小的空容器
	void RemoveSmallEmptyCell(const unsigned int iMinSize);
	
	int GetEmptySubCellCount()const;

	int GetEmptySubCells(CImageCell **pOutBuffer, int iNextIndex);
	
	float TestMargeCellsRate(CImageCell **pMargeBegin, int iCount)const;
	int TestMargeCellsCount(CImageCell **pMargeBegin, int iCount)const;
	
	int MargeCells(CImageCell **pMargeBegin, int iCount);
	void UpdatePosition();
	CLASS_MEMBER(CImageSize , m_ , Size);
	CLASS_MEMBER(ImageCellType, m_, Type);
	CLASS_MEMBER(ImageCellPosition, m_, PositionType);
	CLASS_MEMBER(CPosition, m_, Position);
	CLASS_MEMBER(CImageCell *, m_p, Parent);
	CLASS_MEMBER(CImageInfo *, m_p, SubImage);
	CLASS_MEMBER(CImageCell *, m_p, SubCellLT);
	CLASS_MEMBER(CImageCell *, m_p, SubCellRT);
	CLASS_MEMBER(CImageCell *, m_p, SubCellLB);
	CLASS_MEMBER(CImageCell *, m_p, SubCellRB);
};
