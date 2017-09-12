
// SketcherDoc.cpp : implementation of the CSketcherDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Sketcher.h"
#endif

#include "SketcherDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSketcherDoc

IMPLEMENT_DYNCREATE(CSketcherDoc, CDocument)

BEGIN_MESSAGE_MAP(CSketcherDoc, CDocument)
	ON_COMMAND(ID_COLOR_BLACK, &CSketcherDoc::OnColorBlack)
	ON_COMMAND(ID_COLOR_RED, &CSketcherDoc::OnColorRed)
	ON_COMMAND(ID_COLOR_GREEN, &CSketcherDoc::OnColorGreen)
	ON_COMMAND(ID_COLOR_BLUE, &CSketcherDoc::OnColorBlue)
	ON_COMMAND(ID_ELEMET_LINE, &CSketcherDoc::OnElemetLine)
	ON_COMMAND(ID_ELEMET_RECTANGLE, &CSketcherDoc::OnElemetRectangle)
	ON_COMMAND(ID_ELEMET_CIRCLE, &CSketcherDoc::OnElemetCircle)
	ON_COMMAND(ID_ELEMET_CURVE, &CSketcherDoc::OnElemetCurve)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLACK, &CSketcherDoc::OnUpdateColorBlack)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, &CSketcherDoc::OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, &CSketcherDoc::OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, &CSketcherDoc::OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_ELEMET_LINE, &CSketcherDoc::OnUpdateElemetLine)
	ON_UPDATE_COMMAND_UI(ID_ELEMET_RECTANGLE, &CSketcherDoc::OnUpdateElemetRectangle)
	ON_UPDATE_COMMAND_UI(ID_ELEMET_CIRCLE, &CSketcherDoc::OnUpdateElemetCircle)
	ON_UPDATE_COMMAND_UI(ID_ELEMET_CURVE, &CSketcherDoc::OnUpdateElemetCurve)
END_MESSAGE_MAP()


// CSketcherDoc construction/destruction

CSketcherDoc::CSketcherDoc()
{
	// TODO: add one-time construction code here

}

CSketcherDoc::~CSketcherDoc()
{
}

BOOL CSketcherDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSketcherDoc serialization

void CSketcherDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSketcherDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSketcherDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSketcherDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSketcherDoc diagnostics

#ifdef _DEBUG
void CSketcherDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSketcherDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSketcherDoc commands


void CSketcherDoc::OnColorBlack()
{
	m_Color = ElementColor::BLACK;		// Set the drawing color to black
}


void CSketcherDoc::OnColorRed()
{
	m_Color = ElementColor::RED;		// Set the drawing color to red
}


void CSketcherDoc::OnColorGreen()
{
	m_Color = ElementColor::GREEN;		// Set the drawing color to green
}


void CSketcherDoc::OnColorBlue()
{
	m_Color = ElementColor::BLUE;		// Set the drawing color to blue
}


void CSketcherDoc::OnElemetLine()
{
	m_Element = ElementType::LINE;		// Set element type as a line
}


void CSketcherDoc::OnElemetRectangle()
{
	m_Element = ElementType::RECTANGLE;		// Set element type as a rectangle
}


void CSketcherDoc::OnElemetCircle()
{
	m_Element = ElementType::CIRCLE;		// Set element type as a circle
}


void CSketcherDoc::OnElemetCurve()
{
	m_Element = ElementType::CURVE;		// Set element type as a curve
}

// Add a sketch element
void CSketcherDoc::AddElement(std::shared_ptr<CElement>& pElement)
{
	m_Sketch.push_back(pElement);
}

// Delete a sketch element
void CSketcherDoc::DeleteElement(std::shared_ptr<CElement>& pElement)
{
	m_Sketch.remove(pElement);
}


void CSketcherDoc::OnUpdateColorBlack(CCmdUI *pCmdUI)
{
	// Set menu item Checked if the current color is black
	pCmdUI->SetCheck(m_Color == ElementColor::BLACK);
}


void CSketcherDoc::OnUpdateColorRed(CCmdUI *pCmdUI)
{
	// Set menu item Checked if the current color is red
	pCmdUI->SetCheck(m_Color == ElementColor::RED);
}


void CSketcherDoc::OnUpdateColorGreen(CCmdUI *pCmdUI)
{
	// Set menu item Checked if the current color is green
	pCmdUI->SetCheck(m_Color == ElementColor::GREEN);
}


void CSketcherDoc::OnUpdateColorBlue(CCmdUI *pCmdUI)
{
	// Set menu item Checked if the current color is blue
	pCmdUI->SetCheck(m_Color == ElementColor::BLUE);
}


void CSketcherDoc::OnUpdateElemetLine(CCmdUI *pCmdUI)
{
	// Set Checked if the current element is a line
	pCmdUI->SetCheck(m_Element == ElementType::LINE);
}


void CSketcherDoc::OnUpdateElemetRectangle(CCmdUI *pCmdUI)
{
	// Set Checked if the current element is a rectangle
	pCmdUI->SetCheck(m_Element == ElementType::RECTANGLE);
}


void CSketcherDoc::OnUpdateElemetCircle(CCmdUI *pCmdUI)
{
	// Set Checked if the current element is a circle
	pCmdUI->SetCheck(m_Element == ElementType::CIRCLE);
}


void CSketcherDoc::OnUpdateElemetCurve(CCmdUI *pCmdUI)
{
	// Set Checked if the current element is a curve
	pCmdUI->SetCheck(m_Element == ElementType::CURVE);
}