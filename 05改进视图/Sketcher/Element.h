#pragma once

#include <memory>

static const COLORREF SELECT_COLOR{ RGB(255, 0, 180) };		// Hightlight color

class CElement : public CObject
{
protected:
	CPoint m_StartPoint;		// Element position
	int m_PenWidth;		// Pen width
	COLORREF m_Color;		// Color or an element
	CRect m_EnclosingRect;		// Rectangle enclosing an element
public:
	virtual ~CElement();
	virtual void Draw(CDC* pDC, std::shared_ptr<CElement> pElement=nullptr) {}  // Virtual draw operation

	// Get the element enclosing rectangle 
	const CRect& GetEnclosingRect() const { return m_EnclosingRect; }

protected:
	// Constructors protected so they cannot be called outside the class
	CElement();
	CElement(const CPoint start, COLORREF color, int penWidth = 1);
	// Create a pen
	void CreatePen(CPen& aPen, std::shared_ptr<CElement> pElement)
	{
		if (!aPen.CreatePen(PS_SOLID, m_PenWidth, this == pElement.get() ? SELECT_COLOR : m_Color))
		{
			// Pen create failed
			AfxMessageBox(_T("Pen creation failed."), MB_OK);
			AfxAbort();
		}
	}
};
