// App.h : main header file for the QARIC HURTI application
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QARICHURTI_H__C8633730_1B14_47CE_8681_F63437492323__INCLUDED_)
#define AFX_QARICHURTI_H__C8633730_1B14_47CE_8681_F63437492323__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/////////////////////////////////////////////////////////////////////////////

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CQaricHurti class

class CSpaceAntApp : public CWinApp
{
public:
	CSpaceAntApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQaricHurti)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CQaricHurti)
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CJayliTah dialog

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJayliTah)
	enum { IDD = IDD_ABOUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJayliTah)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJayliTah)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QARICHURTI_H__C8633730_1B14_47CE_8681_F63437492323__INCLUDED_)
