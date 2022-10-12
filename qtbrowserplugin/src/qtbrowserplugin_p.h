// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QTBROWSERPLUGIN_P_H
#define QTBROWSERPLUGIN_P_H

#include <QVariant>
#include <QMutexLocker>
#include <QWidget>

#ifdef Q_WS_X11
#   include <X11/Xlib.h>

class QtNPStream;
class QtNPBindable;
#endif

struct QtNPInstance
{
    NPP npp;

    short fMode;

#ifdef Q_WS_WIN
    typedef HWND Widget;
#endif
#ifdef Q_WS_X11
    typedef Window Widget;
    Display *display;
#endif
#ifdef Q_WS_MAC
    typedef NPPort* Widget;
    QWidget *rootWidget;
#endif

    Widget window;

    QRect geometry;
    QString mimetype;
    QByteArray htmlID;
    union {
        QObject* object;
        QWidget* widget;
    } qt;
    QtNPStream *pendingStream;
    QtNPBindable* bindable;
    QObject *filter;

    QMap<QByteArray, QVariant> parameters;

    qint32 notificationSeqNum;
    QMutex seqNumMutex;
    qint32 getNotificationSeqNum()
        {
            QMutexLocker locker(&seqNumMutex);

            if (++notificationSeqNum < 0)
                notificationSeqNum = 1;
            return notificationSeqNum;
        }

};

#endif // QTBROWSERPLUGIN_P_H
