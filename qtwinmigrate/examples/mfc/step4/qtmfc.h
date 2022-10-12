// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// QtMfc.h : main header file for the WindowsApp application
//

#if !defined(AFX_QTMFC_H__84EEB6F8_ACFE_4691_858C_B335F8EBFF05__INCLUDED_)
#define AFX_QTMFC_H__84EEB6F8_ACFE_4691_858C_B335F8EBFF05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// WindowsApp:
// See QtMfc.cpp for the implementation of this class
//

class WindowsApp : public CWinApp
{
public:
	WindowsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WindowsApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL Run();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(WindowsApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QTMFC_H__84EEB6F8_ACFE_4691_858C_B335F8EBFF05__INCLUDED_)
