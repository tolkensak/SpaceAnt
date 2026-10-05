// MainDoc.h.cpp : implementation of the CSpaceAntDoc class
//

#include "stdafx.h"
#include "App.h"
#include "MainDoc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CGrass::CGrass()
{
}

CGrass::CGrass(CPoint point, INT nRet)
{
	m_nX=point.x;
	m_nY=point.y;
	m_nQueue=nRet;
	m_bNotEaten=TRUE;
}

CGrass::CGrass(INT X, INT Y, INT nRet)
{
	m_nX=X;
	m_nY=Y;
	m_nQueue=nRet;
	m_bNotEaten=TRUE;
}

void CGrass::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar<<m_nX<<m_nY<<m_bNotEaten<<m_nQueue;
	else
		ar>>m_nX>>m_nY>>m_bNotEaten>>m_nQueue;
}

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntDoc

IMPLEMENT_DYNCREATE(CSpaceAntDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpaceAntDoc, CDocument)
	//{{AFX_MSG_MAP(CSpaceAntDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntDoc construction/destruction

CSpaceAntDoc::CSpaceAntDoc()
{
}

CSpaceAntDoc::~CSpaceAntDoc()
{
}

BOOL CSpaceAntDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_bPutGrass=TRUE;
	m_arrGrass.SetSize(0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntDoc serialization

void CSpaceAntDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar<<m_bPutGrass;
	else
		ar>>m_bPutGrass;

	m_arrGrass.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntDoc diagnostics

#ifdef _DEBUG
void CSpaceAntDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpaceAntDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpaceAntDoc commands

void CSpaceAntDoc::Resolve()
{
	m_bPutGrass=FALSE;
	m_arrGrass.FreeExtra();
	INT nCopSani=m_arrGrass.GetSize();

	if(nCopSani==1)
		return;
/////////////////////////////////////
//	find the first grass and base point

	INT nKit=0;
	for(int i=1; i<nCopSani; i++)
		if(m_arrGrass[i].y<m_arrGrass[nKit].y)
			nKit=i;
		else if(m_arrGrass[i].y==m_arrGrass[nKit].y)
			if(m_arrGrass[i].x<m_arrGrass[nKit].x)
				nKit=i;

	if(nKit!=0)
	{
		CGrass cop=m_arrGrass[0];
		m_arrGrass[0]=m_arrGrass[nKit];
		m_arrGrass[nKit]=cop;
	}

	if(nCopSani==2)
		return;

	double cosF,cosF0, aL,bL,cL,b1L,c1L;
///////////////////////
//	find the second grass

	nKit=1;
	aL=pow(m_arrGrass[0].m_nX,2);
	bL=pow(m_arrGrass[1].m_nX-m_arrGrass[0].m_nX,2)+pow(m_arrGrass[1].m_nY-m_arrGrass[0].m_nY,2);
	cL=pow(m_arrGrass[1].m_nX,2)+pow(m_arrGrass[1].m_nY-m_arrGrass[0].m_nY,2);
	cosF0=(cL-bL-aL)/(2*sqrt(aL*bL));
	for(int j=2; j<nCopSani; j++)
	{
		b1L=pow(m_arrGrass[j].m_nX-m_arrGrass[0].m_nX,2)+pow(m_arrGrass[j].m_nY-m_arrGrass[0].m_nY,2);
		c1L=pow(m_arrGrass[j].m_nX,2)+pow(m_arrGrass[j].m_nY-m_arrGrass[0].m_nY,2);
		cosF=(c1L-b1L-aL)/(2*sqrt(aL*b1L));
		if((cosF>cosF0) || (cosF==cosF0) && (b1L<bL))
		{
			nKit=j;
			cosF0=cosF;
			bL=b1L;
		}
	}
	
	if(nKit!=1)
	{
		CGrass cop=m_arrGrass[1];
		m_arrGrass[1]=m_arrGrass[nKit];
		m_arrGrass[nKit]=cop;
	}

	if(nCopSani==3)
		return;
///////////////////////
//	find the other grasses

	for(int i=1; i<nCopSani-1; i++)
	{
		nKit=i+1;
		aL=pow(m_arrGrass[i].x-m_arrGrass[i-1].x,2)+pow(m_arrGrass[i].y-m_arrGrass[i-1].y,2);
		bL=pow(m_arrGrass[i+1].x-m_arrGrass[i].x,2)+pow(m_arrGrass[i+1].y-m_arrGrass[i].y,2);
		cL=pow(m_arrGrass[i+1].x-m_arrGrass[i-1].x,2)+pow(m_arrGrass[i+1].y-m_arrGrass[i-1].y,2);
		cosF0=(cL-bL-aL)/(2*sqrt(aL*bL));
		for(int j=i+2; j<nCopSani; j++)
		{
			b1L=pow(m_arrGrass[j].m_nX-m_arrGrass[i].x,2)+pow(m_arrGrass[j].m_nY-m_arrGrass[i].y,2);
			c1L=pow(m_arrGrass[j].m_nX-m_arrGrass[i-1].x,2)+pow(m_arrGrass[j].m_nY-m_arrGrass[i-1].y,2);
			cosF=(c1L-b1L-aL)/(2*sqrt(aL*b1L));
			if((cosF>cosF0) || (cosF==cosF0) && (b1L<bL))
			{
				nKit=j;
				cosF0=cosF;
				bL=b1L;
			}
		}
		
		if(nKit!=i+1)
		{
			CGrass cop=m_arrGrass[i+1];
			m_arrGrass[i+1]=m_arrGrass[nKit];
			m_arrGrass[nKit]=cop;
		}
	}
}
