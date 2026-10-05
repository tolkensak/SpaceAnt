// MainView.h : interface of the CSpaceAntView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QARICHURTIKORNIS_H__6E55B4E0_8CD4_45AB_A075_396269B2C069__INCLUDED_)
#define AFX_QARICHURTIKORNIS_H__6E55B4E0_8CD4_45AB_A075_396269B2C069__INCLUDED_

#include "Sound.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const INT PARIH=30, CEK=50, ADIM=7, CEKE=7;

class CSpaceAntView : public CScrollView
{
protected: // create from serialization only
	CSpaceAntView();
	DECLARE_DYNCREATE(CSpaceAntView)

// Attributes
public:
	CSpaceAntDoc* GetDocument();

// Operations
public:
	virtual ~CSpaceAntView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpaceAntView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawTrace();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bOpenMouth;
	BOOL *m_pbPutGrass;

	CSound m_sound1;
	CRect m_rcBoard;

	INT	m_nQueue, m_nTimer, m_nGrassCount;

	INT	m_nCX, m_nCY;
	INT m_nAX, m_nAY;
	INT m_nPX, m_nPY;

	INT x, y;
	INT x1, y1;
	INT x2, y2;
	INT dx, dy;

	HICON m_hIconOpenRight,
		  m_hIconOpenLeft,
		  m_hIconCloseRight,
		  m_hIconCloseLeft,
		  m_hIconCorn,
		  m_hIconCornRemnant,
		  m_hIconBugDead,
		  m_hIconBug;
	HCURSOR m_hCursorBug;

	void CreateBoard(CRect& rcBoard);

	CPoint PositionTA(int x, int y)
	{ return CPoint(m_nPX+x*m_nAX, m_nCY-m_nPY-y*m_nAY); }

	CPoint PositionTA(CPoint point)
	{ return CPoint(m_nPX+point.x*m_nAX, m_nCY-m_nPY-point.y*m_nAY); }

	CPoint PositionAT(int x, int y)
	{ return CPoint((x-m_nPX)/m_nAX, (m_nCY-y-m_nPY)/m_nAY); }

	CPoint PositionAT(CPoint point)
	{ return CPoint((point.x-m_nPX)/m_nAX, (m_nCY-point.y-m_nPY)/m_nAY); }

// Generated message map functions
protected:
	//{{AFX_MSG(CSpaceAntView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToolsEat();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateToolsEat(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnToolsPause();
	afx_msg void OnUpdateToolsPause(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Qaric Hurti Kornis.cpp
inline CSpaceAntDoc* CSpaceAntView::GetDocument()
   { return (CSpaceAntDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QARICHURTIKORNIS_H__6E55B4E0_8CD4_45AB_A075_396269B2C069__INCLUDED_)
