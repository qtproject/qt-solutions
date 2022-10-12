// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef GOOGLE_H_INCLUDED
#define GOOGLE_H_INCLUDED
#include <QWidget>
#include <qtsoap.h>

class QLineEdit;
class QTextBrowser;
class QPushButton;

class Google : public QWidget
{
    Q_OBJECT
public:
    Google(QWidget *parent = 0);

private slots:
    void submitRequest();
    void getResponse();

private:
    QtSoapHttpTransport http;

    QLineEdit *searchString;
    QLineEdit *googleKey;
    QTextBrowser *resultView;
    QPushButton *quitButton;
    QPushButton *searchButton;
    QString key;
};

#endif
