#pragma once
#include "Element.h"
class CRectangle :
	public CElement
{
public:
	virtual ~CRectangle();
	virtual void Draw(CDC *pDC) override;

	// Constructor for a rectangle object
	CRectangle(const CPoint& start, const CPoint& end, COLORREF color);

protected:
	CPoint m_BottomRight;		// Bottom-right point for the rectangle
	CRectangle();		// Default constructor - should not be used
};

