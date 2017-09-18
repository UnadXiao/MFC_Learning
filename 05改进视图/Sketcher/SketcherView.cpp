
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

IMPLEMENT_DYNCREATE(CSketcherView, CScrollView)

BEGIN_MESSAGE_MAP(CSketcherView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ELEMENT_MOVE, &CSketcherView::OnElementMove)
	ON_COMMAND(ID_ELEMENT_DELETE, &CSketcherView::OnElementDelete)
END_MESSAGE_MAP()

// CSketcherView construction/destruction

CSketcherView::CSketcherView()
	: m_FirstPoint{ CPoint{} }
{
	// TODO: add construction code here

}

// Create an element of the current type
std::shared_ptr<CElement> CSketcherView::CreateElement() const
{
	// Get a pointer to the document for this view
	CSketcherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		// Verify the pointer is good

	// Get the current element color
	COLORREF color{ static_cast<COLORREF>(pDoc->GetElementColor()) };

	// Now select the element using the type stored in the document
	switch (pDoc->GetElementType())
	{
	case ElementType::RECTANGLE:
		return std::make_shared<CRectangle>(m_FirstPoint, m_SecondPoint, color);

	case ElementType::CIRCLE:
		return std::make_shared<CCircle>(m_FirstPoint, m_SecondPoint, color);

	case ElementType::LINE:
		return std::make_shared<CLine>(m_FirstPoint, m_SecondPoint, color);

	case ElementType::CURVE:
		return std::make_shared<CCurve>(m_FirstPoint, m_SecondPoint, color);

	default:
		// Something's gone wrong
		AfxMessageBox(_T("Bad Element code"), MB_OK);
		AfxAbort();
		return nullptr;
	}
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
	
	// Draw the sketch
	for (auto iter = pDoc->begin(); iter != pDoc->end(); ++iter)
		for (const auto& pElement : *pDoc)
		{
			if (pDC->RectVisible(pElement->GetEnclosingRect()))
				pElement->Draw(pDC, m_pSelected);
		}
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
	if (this == GetCapture())
		ReleaseCapture();		// Stop capturing mouse message
	// Make sure there is an element
	if (m_pTempElement)
	{
		CRect aRect{ m_pTempElement->GetEnclosingRect() };		// Get the enclosing rectangle

		// Add the element pointer to the sketch
		GetDocument()->AddElement(m_pTempElement);
		CClientDC aDC{ this };		// Create a device context
		OnPrepareDC(&aDC);		// Get origin adjusted
		aDC.LPtoDP(aRect);		// Convert to client coordinates;
		InvalidateRect(aRect);		// Get the area redrawn
		m_pTempElement.reset();		// Reset the element pointer
	}
	//CView::OnLButtonUp(nFlags, point);
}


void CSketcherView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC aDC{ this };		// Create a device context
	OnPrepareDC(&aDC);		// Get origin adjusted
	aDC.DPtoLP(&point);		// Convert point to logical coordinates
	m_FirstPoint = point;		// Record the cursor position
	SetCapture();		// Capture subsequent mouse message

	CView::OnLButtonDown(nFlags, point);
}


void CSketcherView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Define a Device Context object for the view
	CClientDC aDC{ this };               // DC is for this view
	OnPrepareDC(&aDC);		// Get origin adjusted
	aDC.DPtoLP(&point);		// Convert point to logical coordinates

	// Verify the left button is down and mouse messages are captured
	if ((nFlags & MK_LBUTTON) && (this == GetCapture()))		// Verify the left button is down
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
		// Create a temporary element of the type and color that
		// is recorded in the document object, and draw it
		m_pTempElement = CreateElement();
		m_pTempElement->Draw(&aDC);
	}
	else
	{
		// We are not creating an element, so do highlighting
		auto pOldSelected = m_pSelected;		// Copy previous
		m_pSelected = GetDocument()->FindElement(point);
		if (m_pSelected != pOldSelected)
		{
			if (m_pSelected)
				GetDocument()->UpdateAllViews(nullptr, 0, m_pSelected.get());
			if (pOldSelected)
				GetDocument()->UpdateAllViews(nullptr, 0, pOldSelected.get());
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CSketcherView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// Invalidate the area corresponding to the element pointed to
	// by the third argument, otherwise invaliadate the whole client area
	if (pHint)
	{
		CClientDC aDC{ this };		// Create a device context
		OnPrepareDC(&aDC);		// Get origin adjusted

		// Get the enclosing rectangle and convert client cooridinates
		CRect aRect{ dynamic_cast<CElement *>(pHint)->GetEnclosingRect() };
    aDC.LPtoDP(aRect);
		InvalidateRect(aRect);		// Get the area redrawn
	}
	else
	{
		InvalidateRect(nullptr);
	}
}


void CSketcherView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize DocSize{ 3000, 3000 };		// Document 30*30ins in MM_LOENGLISH
	
	// Set mapping mode and document size
	SetScrollSizes(MM_LOENGLISH, DocSize);		// Set mapping mode and document size

}


void CSketcherView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_ELEMENT_MENU);		// Load  the context menu
	CMenu *pContext{};
	if (m_pSelected)
	{
		pContext = menu.GetSubMenu(0);
	}
	else
	{
		pContext = menu.GetSubMenu(1);

		// Check color menu items
		ElementColor color{ GetDocument()->GetElementColor() };
		menu.CheckMenuItem(ID_COLOR_BLACK, (ElementColor::BLACK == color ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));
		menu.CheckMenuItem(ID_COLOR_RED, (ElementColor::RED == color ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));
		menu.CheckMenuItem(ID_COLOR_GREEN, (ElementColor::GREEN == color ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));
		menu.CheckMenuItem(ID_COLOR_BLUE, (ElementColor::BLUE == color ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));

		// Check element menu items
		ElementType type{ GetDocument()->GetElementType() };
		menu.CheckMenuItem(ID_ELEMENT_LINE, (ElementType::LINE == type ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));
		menu.CheckMenuItem(ID_ELEMENT_RECTANGLE, (ElementType::RECTANGLE == type ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));
		menu.CheckMenuItem(ID_ELEMENT_CIRCLE, (ElementType::CIRCLE == type ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));
		menu.CheckMenuItem(ID_ELEMENT_CURVE, (ElementType::CURVE == type ? MF_CHECKED : MF_UNCHECKED | MF_BYCOMMAND));



	}
	ASSERT(pContext != nullptr);		// Ensure it's there

	pContext->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CSketcherView::OnElementMove()
{
	// TODO: Add your command handler code here
}


void CSketcherView::OnElementDelete()
{
	if (m_pSelected)
	{
		GetDocument()->DeleteElement(m_pSelected);		// Delete the element
		m_pSelected.reset();
	}
}
