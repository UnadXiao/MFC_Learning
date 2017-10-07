#include "stdafx.h"
#include "Sketcher.h"
#include "Element.h"


IMPLEMENT_SERIAL(CElement, CObject, VERSION_NUMBER)

CElement::CElement()
{
	m_PenWidth = 0;
}

CElement::CElement(const CPoint start, COLORREF color, int penWidth) :
	m_StartPoint{start}, m_Color(color), m_PenWidth(penWidth)
{

}

CElement::~CElement()
{

}

void CElement::Serialize(CArchive &ar)		// Serialize object
{
	CObject::Serialize(ar);		// Call the base class function

	if (ar.IsStoring())
	{
		// Write to the file
		ar << m_StartPoint		// Element position
			<< m_PenWidth		// The pen width
			<< m_Color		// The element color
			<< m_EnclosingRect;		// The enclosing rectangle
	}
	else
	{
		// Reading from the file
		ar >> m_StartPoint		// Element position
			>> m_PenWidth		// The pen width
			>> m_Color		// The element color
			>> m_EnclosingRect;		// The enclosing rectangle
	}
}