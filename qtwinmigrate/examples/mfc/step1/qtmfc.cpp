// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// QtMfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "qtmfc.h"

#include "mainframe.h"
#include <string>
#include <sstream>

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

/////////////////////////////////////////////////////////////////////////////
// WindowsApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static inline void formatWindowsErrorMessage(DWORD errorCode, std::wostream &str)
{
    wchar_t *string = 0;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPWSTR)&string, 0, NULL);
    if (string)
        str << string;
    else
        str << "Unknown error";
    LocalFree((HLOCAL)string);
}

static bool showQtAboutDialog(HWND parentWindow, std::wstring *errorMessage)
{
    typedef BOOL(*pShowDialog)(HWND parent);

    const char dllName[] = "qtdialog.dll";
    const char functionName[] = "showDialog";

    const HMODULE module = LoadLibraryA(dllName);
    const DWORD errorCode = GetLastError();
    if (!module) {
        std::wostringstream str;
        str << "Unable to load '" << dllName << "': Error #" << errorCode << ": ";
        formatWindowsErrorMessage(errorCode, str);
        *errorMessage = str.str();
        return false;
    }

    pShowDialog showDialog = (pShowDialog)GetProcAddress(module, functionName);
    if (!showDialog) {
        std::wostringstream str;
        str << "Unable to resolve function '" << functionName << "' in '"
            << dllName << "'.";
        *errorMessage = str.str();
        return false;
    }

    showDialog(parentWindow);
    FreeLibrary(module);
    return true;
}

// App command to run the dialog
void WindowsApp::OnAppAbout()
{
    const HWND parentWindow = theApp.m_pMainWnd->m_hWnd;
    std::wstring errorMessage;
    if (!showQtAboutDialog(parentWindow, &errorMessage)) {
        MessageBoxW(parentWindow, errorMessage.c_str(), L"QtMfc 1.0 MFC Application",
                    MB_OK | MB_ICONWARNING);
    }
}

/////////////////////////////////////////////////////////////////////////////
// WindowsApp message handlers

