#pragma once
#include "Element.h"

class CText : public CElement
{
	DECLARE_SERIAL(CText)
public:
	virtual ~CText() {}
	virtual void Draw(CDC* pDC, std::shared_ptr<CElement> pElement = nullptr) override;		// Function to display a text
	virtual void Move(const CSize &aSize) override;		// Move a text element
	virtual void Serialize(CArchive &ar) override;		// Serialize object

	// Constructor for a text element
	CText(const CPoint& start, const CPoint& end, const CString& aString, COLORREF color);

protected:
	CString m_String;		// Text to be displayed
	CText() {}
};

