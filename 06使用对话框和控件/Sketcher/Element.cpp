#include "stdafx.h"
#include "Sketcher.h"
#include "Element.h"



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
