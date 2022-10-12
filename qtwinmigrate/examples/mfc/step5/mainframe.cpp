// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <QtGui>

#include "mainframe.h"
#include "childview.h"
#include "ui_optionsdialog.h"

/////////////////////////////////////////////////////////////////////////////
// MainFrame

MainFrame::MainFrame(QWidget *parent)
: QMainWindow(parent)
{
    QMenu *filePopup = menuBar()->addMenu("&File");
    filePopup->addAction("&Exit", this, SLOT(close()));

    QMenu *editPopup = menuBar()->addMenu("&Edit");
    editPopup->addAction("&Undo", 0, 0, Qt::CTRL + Qt::Key_Z);
    editPopup->addSeparator();
    editPopup->addAction("Cu&t", 0, 0, Qt::CTRL + Qt::Key_X);
    editPopup->addAction("&Copy", 0, 0, Qt::CTRL + Qt::Key_C);
    editPopup->addAction("&Paste", 0, 0, Qt::CTRL + Qt::Key_V);
    editPopup->addSeparator();
    editPopup->addAction("&Options...", this, SLOT(editOptions()));

    QMenu *helpPopup = menuBar()->addMenu("&Help");
    helpPopup->addAction("&About QtMfc...", this, SLOT(helpAbout()), Qt::CTRL + Qt::Key_F1);

    view = new ChildView(this);
    setCentralWidget(view);

    statusBar();
}

void MainFrame::editOptions()
{
    QDialog *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    Ui::OptionsDialog ui;
    ui.setupUi(dialog);

    dialog->show();
}

void MainFrame::helpAbout()
{
    QMessageBox::about(this, "About QtMfc", "QtMfc Version 1.0\nCopyright (C) 2003");
}
