// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef POPULATION_H
#define POPULATION_H
#include <qtsoap.h>

class Population : public QObject
{
    Q_OBJECT
public:
    Population(const QString &country, QObject *parent = 0);

private slots:
    void getResponse();

private:
    QtSoapHttpTransport http;
};

#endif
