
// SketcherDoc.h : interface of the CSketcherDoc class
//


#pragma once

#include "Element.h"
#include "ElementType.h"
#include "ElementColor.h"
#include <list>
#include <memory>

using SketchIterator = std::list<std::shared_ptr<CElement>>::const_iterator;




class CSketcherDoc : public CDocument
{
protected: // create from serialization only
	CSketcherDoc();
	DECLARE_DYNCREATE(CSketcherDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CSketcherDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnColorBlack();
	afx_msg void OnColorRed();
	afx_msg void OnColorGreen();
	afx_msg void OnColorBlue();
	afx_msg void OnElemetLine();
	afx_msg void OnElemetRectangle();
	afx_msg void OnElemetCircle();
	afx_msg void OnElemetCurve();

	ElementType GetElementType() const { return m_Element; }
	ElementColor GetElementColor() const { return m_Color; }
	void AddElement(std::shared_ptr<CElement>& pElement);
	void DeleteElement(std::shared_ptr<CElement>& pElement);
	//	Provide a begin iterator for the sketch
	SketchIterator begin() const { return std::begin(m_Sketch); };
	// Provide an end iterator for the sketch
	SketchIterator end() const { return std::end(m_Sketch); };

public:
	std::shared_ptr<CElement> FindElement(const CPoint& point) const;

	int GetPenWidth() const { return m_PenWidth; }		// Get current pen width

	void SendToBack(std::shared_ptr<CElement>& pElement)
	{
		if (pElement)
		{
			m_Sketch.remove(pElement);		// Remove the element from the list
			m_Sketch.push_back(pElement);		// Put it back at the end of the list
		}
	}

protected:
	ElementType m_Element {ElementType::LINE};		// Current element type
	ElementColor m_Color {ElementColor::BLACK};		// Current drawing color
	std::list<std::shared_ptr<CElement>> m_Sketch;		// A list containing the sketch
	int m_PenWidth{};		// Current pen width

public:
	afx_msg void OnUpdateColorBlack(CCmdUI *pCmdUI);
	afx_msg void OnUpdateColorRed(CCmdUI *pCmdUI);
	afx_msg void OnUpdateColorGreen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateColorBlue(CCmdUI *pCmdUI);
	afx_msg void OnUpdateElemetLine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateElemetRectangle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateElemetCircle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateElemetCurve(CCmdUI *pCmdUI);
	afx_msg void OnPenWidth();
};
