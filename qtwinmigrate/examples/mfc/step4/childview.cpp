// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// childview.cpp : implementation of the ChildView class
//

#include "stdafx.h"
#include "qtmfc.h"
#include "childview.h"

#include <qwinwidget.h>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ChildView

ChildView::ChildView()
: widget( 0 )
{
}

ChildView::~ChildView()
{
}


BEGIN_MESSAGE_MAP(ChildView,CWnd )
	//{{AFX_MSG_MAP(ChildView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChildView message handlers

int ChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate( lpCreateStruct ) == -1 )
	return -1;

    widget = new QWinWidget( this );
    QHBoxLayout *hbox = new QHBoxLayout( widget );

    QLabel *label = new QLabel( "Enter text:", widget );
    QLineEdit *edit = new QLineEdit( widget );
    hbox->addWidget( label );
    hbox->addWidget( edit );

    widget->move( 0, 0 );
    widget->show();

    return 0;
}

BOOL ChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void ChildView::OnDestroy()
{
    delete widget;
    widget = 0;

    CWnd::OnDestroy();
}


void ChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	// Do not call CWnd::OnPaint() for painting messages
}

