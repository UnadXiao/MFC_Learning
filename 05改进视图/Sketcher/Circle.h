#pragma once
#include "Element.h"
class CCircle :
	public CElement
{
public:
	virtual ~CCircle();
	virtual void Draw(CDC *pDC, std::shared_ptr<CElement> pElement = nullptr) override;
	CCircle(const CPoint& start, const CPoint& end, COLORREF color);

protected:
	CPoint m_BottomRight;		// Bottom-right point for defining circle
	CCircle();		// Default constructor - should not be used

};

