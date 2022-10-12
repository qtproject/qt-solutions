// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QTSINGLECOREAPPLICATION_H
#define QTSINGLECOREAPPLICATION_H

#include <QCoreApplication>

class QtLocalPeer;

class QtSingleCoreApplication : public QCoreApplication
{
    Q_OBJECT

public:
    QtSingleCoreApplication(int &argc, char **argv);
    QtSingleCoreApplication(const QString &id, int &argc, char **argv);

    bool isRunning();
    QString id() const;

public Q_SLOTS:
    bool sendMessage(const QString &message, int timeout = 5000);


Q_SIGNALS:
    void messageReceived(const QString &message);


private:
    QtLocalPeer* peer;
};

#endif // QTSINGLECOREAPPLICATION_H
