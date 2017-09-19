#pragma once
#include "Element.h"
class CRectangle :
	public CElement
{
public:
	virtual ~CRectangle();
	virtual void Draw(CDC *pDC, std::shared_ptr<CElement> pElement = nullptr) override;
	virtual void Move(const CSize& aSize) override;

	// Constructor for a rectangle object
	CRectangle(const CPoint& start, const CPoint& end, COLORREF color, int penWidth);

protected:
	CPoint m_BottomRight;		// Bottom-right point for the rectangle
	CRectangle();		// Default constructor - should not be used
};

