// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef EASTER_H
#define EASTER_H
#include <qtsoap.h>

class Easter : public QObject
{
    Q_OBJECT
public:
    Easter(short year, QObject *parent = 0);

private slots:
    void getResponse(const QtSoapMessage &);

private:
    QtSoapHttpTransport http;
    int year;
};

#endif
