#include "stdafx.h"
#include "Line.h"

IMPLEMENT_SERIAL(CLine, CElement, VERSION_NUMBER)

CLine::CLine()
{
}


CLine::~CLine()
{
}

void CLine::Draw(CDC * pDC, std::shared_ptr<CElement> pElement)
{
	// Create a pen for this object and initialize it
	CPen aPen;
	CreatePen(aPen, pElement);

	CPen* pOldPen{ pDC->SelectObject(&aPen) };		// Select the pen

	// Now draw the line
	pDC->MoveTo(m_StartPoint);
	pDC->LineTo(m_EndPoint);
	
	pDC->SelectObject(pOldPen);		// Restore the old pen
}

void CLine::Move(const CSize & aSize)
{
	m_StartPoint += aSize;		// Move the start point
	m_EndPoint += aSize;		// and the end point
	m_EnclosingRect += aSize;		// Move the enclosing rectangle
}

CLine::CLine(const CPoint & start, const CPoint & end, COLORREF color, int penWidth) :
	CElement{start, color, penWidth}, m_EndPoint(end)
{
	// Define the enclosing rectangle
	m_EnclosingRect = CRect{ start, end };
	m_EnclosingRect.NormalizeRect();
	int width{ penWidth == 0 ? 1 : penWidth };		// Inflate rect by at least 1
	m_EnclosingRect.InflateRect(width, width);
}

void CLine::Serialize(CArchive &ar)		// Serialize object
{
	CElement::Serialize(ar);		// Call the base class function

	if (ar.IsStoring())
	{
		// Write to the file
		ar << m_EndPoint;		// The end point
	}
	else
	{
		// Reading from the file
		ar >> m_EndPoint;		// The end point
	}
}
