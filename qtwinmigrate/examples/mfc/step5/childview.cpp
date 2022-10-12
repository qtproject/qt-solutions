// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

// childview.cpp : implementation of the ChildView class
//

#include "childview.h"

#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPainter>

/////////////////////////////////////////////////////////////////////////////
// ChildView

ChildView::ChildView(QWidget *parent)
: QWidget(parent)
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Enter text:", widget);
    QLineEdit *edit = new QLineEdit(widget);

    hbox->addWidget(label);
    hbox->addWidget(edit);

    widget->move(0, 0);
    setBackgroundRole(QPalette::Base);
}

void ChildView::paintEvent(QPaintEvent * /*e*/) 
{
    QPainter painter(this);
}
