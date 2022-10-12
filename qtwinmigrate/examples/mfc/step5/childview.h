// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// childview.h : interface of the ChildView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CHILDVIEW_H
#define CHILDVIEW_H

#include <QWidget>

class ChildView : public QWidget
{
public:
    ChildView(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);
};

#endif
