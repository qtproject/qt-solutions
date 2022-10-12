// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// mainframe.h : interface of the MainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QMainWindow>

class ChildView;

class MainFrame : public QMainWindow
{
    Q_OBJECT
public:
    MainFrame(QWidget *parent = 0);

protected Q_SLOTS:
    void editOptions();
    void helpAbout();

private:
    ChildView *view;
};

#endif
