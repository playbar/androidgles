#include "ImageInfo.h"
#include "ImageCell.h"
#ifdef WIN32
string&   replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}
bool operator ==(const TEXTURE_ID &A, const TEXTURE_ID &B)
{
	return A.m_pImage == B.m_pImage && A.m_szFileName == B.m_szFileName;
}
#endif
CImageInfo::CImageInfo(unsigned int uiWidth , unsigned int uiHeight ) :
m_SrcPosition(0, 0), 
m_Size(uiWidth, uiHeight),
m_pCell(NULL),
m_iTextureID(0)
{
}
CImageInfo::CImageInfo(unsigned int uiX, unsigned int uiY, unsigned int uiWidth, unsigned int uiHeight , TEXTURE_ID iTextureID, int index):
m_SrcPosition(uiX, uiY),
m_Size(uiWidth, uiHeight),
m_pCell(NULL),
m_iTextureID(iTextureID),
m_iIndex(index)
{

}

CImageInfo::~CImageInfo()
{
}

CImageSize CImageInfo::CalculateCellSize()const
{
	unsigned int iWidth = 1, iHeight = 1;
	while (iWidth < m_Size.GetWidth())
	{
		iWidth <<= 1;
	}
	while (iHeight < m_Size.GetHeight())
	{
		iHeight <<= 1;
	}
	return CImageSize(iWidth , iHeight);
}

CImageSize CImageInfo::CalculateMinCellSize()const
{
	return m_Size.Alignment(4);
}
int CImageInfo::GetMaxEdge()const
{
	int MaxEdge = (int)fmax(m_Size.GetWidth(), m_Size.GetHeight());
	int iRet = 1;
	while (iRet < MaxEdge)
	{
		iRet <<= 1;
	}
	return iRet;
}

CPosition CImageInfo::GetPosition()const
{
	return m_pCell->GetPosition();
}
#ifndef WIN32
#define CIMAGEINFO_FMT "{\n\"SubTextureID\":%d,\n\"Index\":%d,\n\"Position\":[%d,%d],\n\"Size\":[%d,%d]\n}\n"
#else
#define CIMAGEINFO_FMT "{\n\"SubTextureID\":\"%s\",\n\"Position\":[%d,%d],\n\"Size\":[%d,%d]\n}\n"
#endif
int CImageInfo::PrintScript(char * szOutBuffer) const
{
	CPosition BakedPosition = m_pCell->GetPosition();
#ifndef WIN32
	return sprintf(szOutBuffer, CIMAGEINFO_FMT, 
		m_iTextureID, 
		m_iIndex,
		BakedPosition.GetX(), 
		BakedPosition.GetY(),
		m_Size.GetWidth() , 
		m_Size.GetHeight());
#else
	//Ìæ»»µô\Îª\\;
	string tmpFileName(m_iTextureID.c_str());
	replace_all_distinct(tmpFileName, "\\", "\\\\");
	int i = 0;
	i++;
	return sprintf(szOutBuffer, CIMAGEINFO_FMT,
		tmpFileName.c_str()
		, BakedPosition.GetX(), BakedPosition.GetY(),
		m_Size.GetWidth(), m_Size.GetHeight());
	// strlen(szOutBuffer);
#endif
}
#ifndef WIN32
void CImageInfo::DoBaker(int iTargerTextureID, int iCurrentSourceTexture)const
{

	if (m_iTextureID && glIsTexture(m_iTextureID))
	{
		if (iCurrentSourceTexture != GetTextureID())
		{
			glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetTextureID(), 0);
			
			glBindTexture(GL_TEXTURE_2D, iTargerTextureID);
		}
		CPosition ptTarget = GetPosition();
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, ptTarget.GetX(), ptTarget.GetY(), m_SrcPosition.GetX(), m_SrcPosition.GetY(), m_Size.GetWidth(), m_Size.GetHeight());
	}
}
#else
void CImageInfo::DoBaker(Graphics * pGraphics)const
{
	CPosition pt = GetPosition();

		SolidBrush BR(Color(255, 128, 128));
		Rect rcImage(pt.GetX(), pt.GetY(), m_Size.GetWidth(), m_Size.GetHeight());
		//pGraphics->FillRectangle(&BR, rcImage);
		//
		//m_iTextureID.m_pImage->GetPixelFormat();
		//m_iTextureID.m_pImage->GetWidth();
		//m_iTextureID.m_pImage->GetHeight();

		//SizeF szPhysicalDimensionSize(0,0);
		//Status  St = m_iTextureID.m_pImage->GetPhysicalDimension(&szPhysicalDimensionSize);

		pGraphics->DrawImage(m_iTextureID.m_pImage , rcImage);
		//pGraphics->DrawImage(m_iTextureID.m_pImage,
		//	(int)pt.GetX(), (int)pt.GetY(),
		//	(int)m_SrcPosition.GetX(), (int)m_SrcPosition.GetY(),
		//	(int)m_Size.GetWidth(), (int)m_Size.GetHeight(),
		//	UnitPixel);
}
#endif

void CImageInfo::UpdateBakedPosition(TEXTURE_ID iBakedTextureID)
{
	m_SrcPosition = GetPosition();
	SetTextureID(iBakedTextureID);
}