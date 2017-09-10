
// SketcherView.cpp : implementation of the CSketcherView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Sketcher.h"
#endif

#include "SketcherDoc.h"
#include "SketcherView.h"
#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Curve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSketcherView

IMPLEMENT_DYNCREATE(CSketcherView, CView)

BEGIN_MESSAGE_MAP(CSketcherView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CSketcherView construction/destruction

CSketcherView::CSketcherView()
	: m_FirstPoint{ CPoint{} }
{
	// TODO: add construction code here

}

CSketcherView::~CSketcherView()
{
}

BOOL CSketcherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSketcherView drawing

void CSketcherView::OnDraw(CDC* pDC)
{
	CSketcherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->MoveTo(50, 50);
	pDC->LineTo(50, 200);
	pDC->LineTo(150, 200);
	pDC->LineTo(150, 50);
	pDC->LineTo(50, 50);
	
	// Declare a pen object and initialize it as
	// a red solid pen drawing a line 2 pixels wide
	CPen aPen;
	aPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	CPen *pOldPen{ pDC->SelectObject(&aPen) };		// Select aPen as the pen

	pDC->Ellipse(50, 50, 150, 150);		// Draw the 1st (large) circle

	// Define the bounding rectangle for the 2nd (smaller) circle
	CRect rect{ 250, 50, 300, 100 };
	CPoint start{ 275, 100 };
	CPoint end{ 250, 75 };
	pDC->Arc(&rect, start, end);
	pDC->SelectObject(pOldPen);
}


// CSketcherView printing

BOOL CSketcherView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSketcherView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSketcherView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSketcherView diagnostics

#ifdef _DEBUG
void CSketcherView::AssertValid() const
{
	CView::AssertValid();
}

void CSketcherView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSketcherDoc* CSketcherView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSketcherDoc)));
	return (CSketcherDoc*)m_pDocument;
}
#endif //_DEBUG


// CSketcherView message handlers


void CSketcherView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
}


void CSketcherView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_FirstPoint = point;		// Record the cursor position

	CView::OnLButtonDown(nFlags, point);
}


void CSketcherView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Define a Device Context object for the view
	CClientDC aDC{ this };               // DC is for this view

	if (nFlags & MK_LBUTTON)		// Verify the left button is down
	{
		m_SecondPoint = point;		// Save the current cursor position
		if (m_pTempElement)
		{
			// An element was created previously
			if (ElementType::CURVE == GetDocument()->GetElementType())   // A curve?
			{  // We are drawing a curve so add a segment to the existing curve
				std::dynamic_pointer_cast<CCurve>(m_pTempElement)->AddSegment(m_SecondPoint);
				m_pTempElement->Draw(&aDC);     // Now draw it
				return;                         // We are done
			}
			else
			{
				// If we get to here it's not a curve so
				// redraw the old element so it disappears from the view
				aDC.SetROP2(R2_NOTXORPEN);       // Set the drawing mode
				m_pTempElement->Draw(&aDC);      // Redraw the old element to erase it
			}
		}
		// Test for a previous temporary element
		{
			// We get to here if there was a previous mouse move
			// so add code to delete the old element
		}
		// Add code to create new element
		// and cause it to be drawn
	}

	CView::OnMouseMove(nFlags, point);
}