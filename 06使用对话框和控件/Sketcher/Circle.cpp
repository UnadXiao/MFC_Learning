#include "stdafx.h"
#include "Circle.h"


CCircle::CCircle()
{
}


CCircle::~CCircle()
{
}

// Draw a circle
void CCircle::Draw(CDC *pDC, std::shared_ptr<CElement> pElement)
{
	// Create a pen for this object and initialize it
	CPen aPen;
	CreatePen(aPen, pElement);

	CPen* pOldPen{ pDC->SelectObject(&aPen) };		// Select a pen
	CBrush* pOldBrush{ dynamic_cast<CBrush *>(pDC->SelectStockObject(NULL_BRUSH)) };		// Select a null bursh

	// Now draw the circle
	pDC->Ellipse(m_StartPoint.x, m_StartPoint.y, m_BottomRight.x, m_BottomRight.y);

	pDC->SelectObject(pOldBrush);		// Restore the old brush
	pDC->SelectObject(pOldPen);		// Restore the old pen
}

void CCircle::Move(const CSize & aSize)
{
	m_StartPoint += aSize;		// Move the start point
	m_BottomRight += aSize;		// and the bottom right point
	m_EnclosingRect += aSize;		// Move the enclosing rectangle
}

// Constructor for a circle object
CCircle::CCircle(const CPoint & start, const CPoint & end, COLORREF color, int penWidth) :
	CElement{start, color, penWidth}
{
	// Calculate the radius using floating-point values
	// because that is requried by sqrt() function (in cmath)
	long radius{ static_cast<long> (sqrt(
		static_cast<double>((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y)))) };
	if (radius < 1L) radius = 1L;		// Circle radius must be >= 1

	// Define left-top and right-bottom points of rectangle for MM_TEXT mode
	m_StartPoint = CPoint{ start.x - radius, start.y - radius };
	m_BottomRight = CPoint{ start.x + radius, start.y + radius };

	// Define the enclosing rectangle
	m_EnclosingRect = CRect{ m_StartPoint.x, m_StartPoint.y, m_BottomRight.x, m_BottomRight.y };
	int width{ penWidth == 0 ? 1 : penWidth };
	m_EnclosingRect.InflateRect(width, width);
}
