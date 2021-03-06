#pragma once
#include "Element.h"
class CLine :
	public CElement
{
	DECLARE_SERIAL(CLine)
public:
	virtual ~CLine();
	virtual void Draw(CDC *pDC, std::shared_ptr<CElement> pElement = nullptr) override;		// Function to display a line
	virtual void Move(const CSize& aSize) override;		// Function to move an element
	virtual void Serialize(CArchive &ar) override;		// Serialize object

	// Constructor for a line object
	CLine(const CPoint& start, const CPoint& end, COLORREF Color, int penWidth);

protected:
	CPoint m_EndPoint;		// End point of line

protected:
	CLine();		// Default constructor should not be used;

};

