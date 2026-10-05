// MainDoc.h : interface of the CSpaceAntDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QARICHURTIHUJAT_H__662AC3FC_CA94_4A14_932F_615635C67E29__INCLUDED_)
#define AFX_QARICHURTIHUJAT_H__662AC3FC_CA94_4A14_932F_615635C67E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////

class CGrass
{
public:
	CGrass();
	CGrass(CPoint point, INT nRet);
	CGrass(INT X, INT Y, INT nRet);

	CPoint Position() { return CPoint(x, y); }
	virtual void Serialize(CArchive& ar);

	INT x, y, m_nQueue;
	BOOL m_bNotEaten;
};

/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

class CSpaceAntDoc : public CDocument
{
protected: // create from serialization only
	CSpaceAntDoc();
	DECLARE_DYNCREATE(CSpaceAntDoc)

public:
	BOOL m_bPutGrass;
	CArray <CGrass, CGrass> m_arrGrass;

	void Resolve();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpaceAntDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpaceAntDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CSpaceAntDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QARICHURTIHUJAT_H__662AC3FC_CA94_4A14_932F_615635C67E29__INCLUDED_)
