// Qaric Hurti Kornis.cpp : implementation of the CSpaceAntView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "App.h"
#include "MainDoc.h"
#include "MainView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntView

IMPLEMENT_DYNCREATE(CSpaceAntView, CScrollView)

BEGIN_MESSAGE_MAP(CSpaceAntView, CScrollView)
	//{{AFX_MSG_MAP(CSpaceAntView)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TOOLS_PLAY, OnToolsEat)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PLAY, OnUpdateToolsEat)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_COMMAND(ID_TOOLS_PAUSE, OnToolsPause)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PAUSE, OnUpdateToolsPause)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntView construction/destruction

CSpaceAntView::CSpaceAntView()
{
	m_hCursorBug=AfxGetApp()->LoadCursor(IDC_FOOD);

	m_hIconOpenRight=AfxGetApp()->LoadIcon(IDI_OPEN_RIGHT);
	m_hIconOpenLeft=AfxGetApp()->LoadIcon(IDI_OPEN_LEFT);
	m_hIconCloseRight=AfxGetApp()->LoadIcon(IDI_CLOSE_RIGHT);
	m_hIconCloseLeft=AfxGetApp()->LoadIcon(IDI_CLOSE_LEFT);
	m_hIconBugDead=AfxGetApp()->LoadIcon(IDI_DEATH);
	m_hIconCorn=AfxGetApp()->LoadIcon(IDI_FOOD);
	m_hIconCornRemnant=AfxGetApp()->LoadIcon(IDI_RIND);

	m_hIconBug=NULL;
	m_nTimer=-1;
	m_pbPutGrass=NULL;
}

CSpaceAntView::~CSpaceAntView()
{
}

void CSpaceAntView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(260, 260));

	CSpaceAntDoc* pDoc=GetDocument();
	m_pbPutGrass=&pDoc->m_bPutGrass;
	m_nGrassCount=pDoc->m_arrGrass.GetSize();
	
	CRect rc;
	GetClientRect(&rc);
	CreateBoard(rc);
}

void CSpaceAntView::OnDestroy() 
{
	m_pbPutGrass=NULL;
	
	KillTimer(1);
	KillTimer(2);
	m_nTimer=-1;
	
	CScrollView::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntView drawing

BOOL CSpaceAntView::OnEraseBkgnd(CDC* pDC) 
{
//	CScrollView::OnEraseBkgnd(pDC);

	CRect rc;
	GetClientRect(&rc);
	CBitmap bmSpace;
	bmSpace.LoadBitmap(IDB_SPACE);
	CBrush ctQaric(&bmSpace);
	pDC->FillRect(&rc, &ctQaric);

	return TRUE;
}

void CSpaceAntView::OnDraw(CDC* pDC)
{	
	COLORREF clrGray(RGB(125, 125, 125)),
			 clrTextOrigin=pDC->SetTextColor(clrGray),
			 clrBKOrigin=pDC->SetBkColor(RGB(0, 0, 0));

	CPen penThick(PS_SOLID, 2, clrGray);	
	CPen* penOrig=pDC->SelectObject(&penThick);

	pDC->MoveTo(PositionTA(0, CEK));
	pDC->LineTo(PositionTA(0, 0));
	pDC->LineTo(PositionTA(CEK, 0));
	pDC->TextOut(PositionTA(0, 0).x-18, PositionTA(0, 0).y+4, "0");

	CPen penDotted(PS_DOT, 1, clrGray);
	pDC->SelectObject(&penDotted);

	CString str;
	CPoint point;
	for(int i=10; i<=CEK; i+=10)
	{
		str.Format(_T("%d"), i);

		pDC->MoveTo(PositionTA(0, i));
		point=pDC->GetCurrentPosition();
		pDC->TextOut(point.x-22, point.y-7, str);
		pDC->LineTo(PositionTA(CEK, i));

		pDC->MoveTo(PositionTA(i, 0));
		point=pDC->GetCurrentPosition();
		pDC->TextOut(point.x-7, point.y+5, str);
		pDC->LineTo(PositionTA(i, CEK));
	}


	CPen penRed(PS_SOLID, 2, RGB(255, 0, 0));
	pDC->SelectObject(&penRed);


	CSpaceAntDoc* pDoc=GetDocument();
	if(m_nGrassCount>0)
		pDC->MoveTo(PositionTA(0, pDoc->m_arrGrass[0].m_nY));

	CGrass* pGrass;
	for(int i=0; i<m_nGrassCount; i++)
	{
		pGrass=&pDoc->m_arrGrass[i];
		point=PositionTA(pGrass->Position());

		if(pGrass->m_bNoeEaten)
			pDC->DrawIcon(point.x-CEKE, point.y-CEKE, m_hIconCorn);
		else
		{
			pDC->LineTo(point);
			pDC->DrawIcon(point.x-CEKE, point.y-CEKE, m_hIconCornRemnant);
		}
	}

	pDC->SelectObject(penOrig);
	pDC->SetBkColor(clrBKOrigin);
	pDC->SetTextColor(clrTextOrigin);

	if(m_nTimer!=1)
		pDC->DrawIcon(x-7, y-7, m_hIconBug);
}

BOOL CSpaceAntView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	Invalidate(FALSE);

	CRect rc;
	GetClientRect(&rc);

	dx+=sizeScroll.cx;
	if(dx<0)
		dx=0;
	else if(dx>260-rc.Width())
		dx=260-rc.Width();

	dy+=sizeScroll.cy;
	if(dy<0)
		dy=0;
	else if(dy>260-rc.Height())
		dy=260-rc.Height();

	return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntView printing

BOOL CSpaceAntView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSpaceAntView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CSpaceAntView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntView diagnostics

#ifdef _DEBUG
void CSpaceAntView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSpaceAntView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSpaceAntDoc* CSpaceAntView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpaceAntDoc)));
	return (CSpaceAntDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

void CSpaceAntView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(*m_pbPutGrass && m_rcBoard.PtInRect(point))
		SetCursor(m_hCursorBug);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CSpaceAntView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(*m_pbPutGrass && m_rcBoard.PtInRect(point))
	{
		if(m_nGrassCount<100)
		{
			CPoint point(point.x+dx, point.y+dy);
			SetCursor(m_hCursorBug);
			point=PositionAT(point);
			GetDocument()->m_arrGrass.Add(CGrass(point, m_nGrassCount++));
			point=PositionTA(point);
			CClientDC dc(this);
			dc.DrawIcon(point.x-7-dx, point.y-7-dy, m_hIconCorn);
		}
		else if(MessageBox(_T("Food count can not be more than 100 !\n Continue with these 100 ?"), _T("Warning"), MB_ICONQUESTION | MB_YESNO)==IDYES) {
            OnToolsEat();
        }
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CSpaceAntView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(*m_pbPutGrass)
	{
		CPoint point(point.x+dx, point.y+dy);
		CString str;
		if(m_rcBoard.PtInRect(point))
		{
			SetCursor(m_hCursorBug);
			point=PositionAT(point);
			str.Format(_T(" Location:  x = %d,  y = %d"), point.x, point.y);
		}

		CStatusBar* pStatusBar=(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		if(pStatusBar)
			pStatusBar->SetPaneText(1, str);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CSpaceAntView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	if(m_pbPutGrass==NULL)
		return;

	CPoint n, n1,n2;
	n=PositionAT(x, y);
	n1=PositionAT(x1, y1);
	n2=PositionAT(x2, y2);

	CRect rc;
	GetClientRect(&rc);
	CreateBoard(rc);

	n=PositionTA(n);
	n1=PositionTA(n1);
	n2=PositionTA(n2);

	x=n.x, y=n.y;
	x1=n1.x, y1=n1.y;
	x2=n2.x, y2=n2.y;
}

void CSpaceAntView::CreateBoard(CRect& rcBoard)
{
	m_nCX=rcBoard.Width()<260?260:rcBoard.Width();
	m_nCY=rcBoard.Height()<260?260:rcBoard.Height();

	INT nWidth=m_nCX-2*PARIH;
	m_nAX=nWidth/CEK;
	m_nPX=PARIH+(nWidth%CEK)/2;
	
	nWidth=m_nCY-2*PARIH;
	m_nAY=nWidth/CEK;
	m_nPY=PARIH+(nWidth%CEK)/2;

	m_rcBoard=CRect(PositionTA(0, CEK), PositionTA(CEK, 0));
	m_rcBoard+=CRect(3, 2, 3, 2);

	dx=dy=0;
}

void CSpaceAntView::DrawTrace()
{
	CSpaceAntDoc* pDoc=GetDocument();

	CString str;
	m_bOpenMouth=FALSE;

	if(m_nQueue==0)
	{
		for(int i=0; i<m_nGrassCount; i++)
			pDoc->m_arrGrass[i].m_bNoeEaten=TRUE;

		CPoint point=PositionTA(0, pDoc->m_arrGrass[0].m_nY);
		x=x1=point.x;
		y=y1=point.y;

		CGrass* pGrass=&pDoc->m_arrGrass[m_nQueue];
		point=PositionTA(pGrass->m_nX, pGrass->m_nY);
		x2=point.x;
		y2=point.y;

		str.Format(_T(" Now (%d, %d)"), pGrass->m_nX, pGrass->m_nY);
	}
	else
	{
		pDoc->m_arrGrass[m_nQueue-1].m_bNoeEaten=FALSE;

		if(m_nQueue<m_nGrassCount)
		{
			CGrass* pGrass=&pDoc->m_arrGrass[m_nQueue-1];
			CPoint point=PositionTA(pGrass->m_nX, pGrass->m_nY);
			x=x1=point.x;
			y=y1=point.y;

			pGrass=&pDoc->m_arrGrass[m_nQueue];
			point=PositionTA(pGrass->m_nX, pGrass->m_nY);
			x2=point.x;
			y2=point.y;

			str.Format(_T(" Now (%d, %d)"), pGrass->m_nX, pGrass->m_nY);
		}
		else
		{
			KillTimer(1);
			m_nTimer=-1;
			str=_T(" No any food!");
			m_hIconBug=NULL;

			m_sound1.Load(IDV_DEATH, AfxGetInstanceHandle());
			m_sound1.Play();

			m_nQueue=0;
			SetTimer(2, 100, NULL);
		}
	}

	CStatusBar* pStatusBar=(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	if(pStatusBar)
		pStatusBar->SetPaneText(1, str);
}

void CSpaceAntView::OnToolsEat()
{
	KillTimer(1);
	KillTimer(2);
	m_nTimer=-1;

	m_sound1.Load(IDV_SPACE, AfxGetInstanceHandle());
	m_sound1.Play();

	CSpaceAntDoc* pDoc=GetDocument();
	if(*m_pbPutGrass)
		pDoc->Resolve();
	
	m_nQueue=0;
	DrawTrace();

	Invalidate();
	m_nTimer=SetTimer(1, 150, NULL);
}

void CSpaceAntView::OnUpdateToolsEat(CCmdUI* pCmdUI)
{
	if(m_nGrassCount==0)
		pCmdUI->Enable(FALSE);
}

void CSpaceAntView::OnToolsPause() 
{
	if(m_nTimer)
		m_nTimer=!KillTimer(m_nTimer);
	else
		m_nTimer=SetTimer(1, 150, NULL);
}

void CSpaceAntView::OnUpdateToolsPause(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_nTimer);

	if(m_nTimer==-1)
		pCmdUI->Enable(FALSE);
}

void CSpaceAntView::OnTimer(UINT nIDEvent) 
{
	CClientDC dc(this);

	CPoint point[6];
	int nPointCount=6, x0=x, y0=y;

	dc.FillSolidRect(x0-7-dx, y0-7-dy, 13, 15, RGB(0, 0, 0));

	if(nIDEvent==1)
	{
		if(x2>x1)
		{
			double a=atan((double)(y2-y1)/(double)(x2-x1));
			if(a<-1)
			{
				y+=(int)(ADIM*sin(a));
				if(y<y2)
					y=y2;
				x=(y-y1)*(x2-x1)/(y2-y1)+x1;
			}
			else if(a>1)
			{
				y+=(int)(ADIM*sin(a));
				if(y>y2)
					y=y2;
				x=(y-y1)*(x2-x1)/(y2-y1)+x1;
			}
			else
			{
				x+=(int)(ADIM*cos(a));
				if(x>x2)
					x=x2;
				y=(x-x1)*(y2-y1)/(x2-x1)+y1;
			}

			if(m_bOpenMouth)
				dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconCloseRight);
			else
				dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconOpenRight);

			if(y1>y2)
			{
				point[0]=CPoint(x0-7-dx, y-7-dy);
				point[1]=CPoint(x-7-dx, y-7-dy);
				point[2]=CPoint(x-7-dx, y+8-dy);
				point[3]=CPoint(x+6-dx, y+8-dy);
				point[4]=CPoint(x+6-dx, y0+8-dy);
				point[5]=CPoint(x0-7-dx, y0+8-dy);
			}
			else if(y1<y2)
			{
				point[0]=CPoint(x0-7-dx, y0-7-dy);
				point[1]=CPoint(x+6-dx, y0-7-dy);
				point[2]=CPoint(x+6-dx, y-7-dy);
				point[3]=CPoint(x-7-dx, y-7-dy);
				point[4]=CPoint(x-7-dx, y+8-dy);
				point[5]=CPoint(x0-7-dx, y+8-dy);
			}
			else
			{
				nPointCount=4;
				point[0]=CPoint(x0-7-dx, y0-7-dy);
				point[1]=CPoint(x-7-dx, y-7-dy);
				point[2]=CPoint(x-7-dx, y+8-dy);
				point[3]=CPoint(x0-7-dx, y0+8-dy);
			}
		}
		else if(x1>x2)
		{
			double a=atan((double)(y2-y1)/(double)(x2-x1));
			if(a<-1)
			{
				y-=(int)(ADIM*sin(a));
				if(y>y2)
					y=y2;
				x=(y-y1)*(x2-x1)/(y2-y1)+x1;
			}
			else if(a>1)
			{
				y-=(int)(ADIM*sin(a));
				if(y<y2)
					y=y2;
				x=(y-y1)*(x2-x1)/(y2-y1)+x1;
			}
			else
			{
				x-=(int)(ADIM*cos(a));
				if(x<x2)
					x=x2;
				y=(x-x1)*(y2-y1)/(x2-x1)+y1;
			}

			if(m_bOpenMouth)
				dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconCloseLeft);
			else
				dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconOpenLeft);

			if(y1>y2)
			{
				point[0]=CPoint(x+6-dx, y-7-dy);
				point[1]=CPoint(x0+6-dx, y-7-dy);
				point[2]=CPoint(x0+6-dx, y0+9-dy);
				point[3]=CPoint(x-7-dx, y0+8-dy);
				point[4]=CPoint(x-7-dx, y+8-dy);
				point[5]=CPoint(x+6-dx, y+8-dy);
			}
			else if(y1<y2)
			{
				point[0]=CPoint(x-7-dx, y0-7-dy);
				point[1]=CPoint(x0+6-dx, y0-7-dy);
				point[2]=CPoint(x0+6-dx, y+8-dy);
				point[3]=CPoint(x+6-dx, y+8-dy);
				point[4]=CPoint(x+6-dx, y-7-dy);
				point[5]=CPoint(x-7-dx, y-7-dy);
			}
			else
			{
				nPointCount=4;
				point[0]=CPoint(x+6-dx, y-7-dy);
				point[1]=CPoint(x0+6-dx, y0-7-dy);
				point[2]=CPoint(x0+6-dx, y0+8-dy);
				point[3]=CPoint(x+6-dx, y+8-dy);
			}
		}
		else
		{
			if(y2>y1)
			{
				y+=ADIM;
				if(y>y2)
					y=y2;

				nPointCount=4;
				point[0]=CPoint(x0-7-dx, y0-7-dy);
				point[1]=CPoint(x0+6-dx, y0-7-dy);
				point[2]=CPoint(x+6-dx, y-7-dy);
				point[3]=CPoint(x-7-dx, y-7-dy);

				if(m_bOpenMouth)
					dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconCloseRight);
				else
					dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconOpenRight);
			}
			else if(y1>y2)
			{
				y-=ADIM;
				if(y<y2)
					y=y2;

				nPointCount=4;
				point[0]=CPoint(x-7-dx, y+8-dy);
				point[1]=CPoint(x+6-dx, y+8-dy);
				point[2]=CPoint(x0+6-dx, y0+8-dy);
				point[3]=CPoint(x0-7-dx, y0+8-dy);

				if(m_bOpenMouth)
					dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconCloseLeft);
				else
					dc.DrawIcon(x-7-dx, y-7-dy, m_hIconBug=m_hIconOpenLeft);
			}
		}

		m_bOpenMouth=!m_bOpenMouth;

		if(x==x2 && y==y2)
		{
			CPen penRed(PS_SOLID, 2, RGB(255, 0, 0));
			CPen* penOrig=dc.SelectObject(&penRed);
			dc.MoveTo(x1-dx, y1-dy);
			dc.LineTo(x2-dx, y2-dy);
			dc.SelectObject(penOrig);
			m_nQueue++;
			DrawTrace();
		}
	}
	else
	{
		y+=m_nQueue;
		m_nQueue++;
		if(y-7-dx>m_rcBoard.bottom+m_nPY)
		{
			KillTimer(2);
			CStatusBar* pStatusBar=(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
			if(pStatusBar)
				pStatusBar->SetPaneText(1, _T(""));
		}

		dc.DrawIcon(x-7-dx, y-7-dy, 	m_hIconBugDead);
		
		nPointCount=4;
		point[0]=CPoint(x0-7-dx, y0-7-dy);
		point[1]=CPoint(x0+6-dx, y0-7-dy);
		point[2]=CPoint(x+6-dx, y-7-dy);
		point[3]=CPoint(x-7-dx, y-7-dy);
	}

	CRgn rgn;
	rgn.CreatePolygonRgn(point, nPointCount, ALTERNATE);
	InvalidateRgn(&rgn, FALSE);

	CScrollView::OnTimer(nIDEvent);
}
