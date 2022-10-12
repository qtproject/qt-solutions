// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QTUNIXSERVERSOCKET_H
#define QTUNIXSERVERSOCKET_H

#include <QTcpServer>

class QtUnixServerSocket : public QTcpServer
{
    Q_OBJECT
public:
    QtUnixServerSocket(const QString &path, QObject *parent = 0);
    QtUnixServerSocket(QObject *parent = 0);

    void setPath(const QString &path);
    void close();

private:
    QString path_;
};


#endif
