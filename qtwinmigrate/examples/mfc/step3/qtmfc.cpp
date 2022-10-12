// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// QtMfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "qtmfc.h"

#include "mainframe.h"

#include <qmfcapp.h>
#include <qwinwidget.h>
#include <QMessageBox>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WindowsApp

BEGIN_MESSAGE_MAP(WindowsApp, CWinApp)
	//{{AFX_MSG_MAP(WindowsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WindowsApp construction

WindowsApp::WindowsApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only WindowsApp object

WindowsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// WindowsApp initialization

BOOL WindowsApp::InitInstance()
{
	// Standard initialization

#if _MFC_VER < 0x0700
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));



	MainFrame* pFrame = new MainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);



	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

BOOL WindowsApp::Run()
{
    return QMfcApp::run( this );
}

/////////////////////////////////////////////////////////////////////////////
// WindowsApp message handlers


// App command to run the dialog
void WindowsApp::OnAppAbout()
{
    QWinWidget win( theApp.m_pMainWnd );
    win.showCentered();
    QMessageBox::about( &win, "About QtMfc", "QtMfc Version 1.0\nCopyright (C) 2003" );
}
