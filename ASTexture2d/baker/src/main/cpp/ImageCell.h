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
	// ͨ�����ڵ㴫���λ�ã��𼶵ݹ�������е��ӽڵ�
	void UpdatePosition(CPosition ptNewPosition);
	// ��ֵ�Ԫ��
	int Split(const unsigned int &iSize, const bool &bLeftToRight, const unsigned int iSizeLimit = 16);
	// ��һ��С�ĵ�Ԫ��ϲ�����ǰ��Ԫ�����档
	CImageCell * MargeIn(CImageCell *pSubCell , bool bLeftToRight , int iSizeLimit = 16);
	// �ѱ��ڵ�������ӽڵ��У���С�ĵ�Ԫ��ɾ��������ʡһЩ�ռ�
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
	// ɾ����С�Ŀ�����
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
