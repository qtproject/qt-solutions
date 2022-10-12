// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QTUNIXSOCKET_H
#define QTUNIXSOCKET_H

#include <QTcpSocket>

class QtUnixSocket : public QTcpSocket
{
    Q_OBJECT
public:
    QtUnixSocket(QObject *parent = 0);

    bool connectTo(const QString &path);
};

#endif
