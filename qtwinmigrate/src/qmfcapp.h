// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


// Declaration of the QMfcApp classes

#ifndef QMFCAPP_H
#define QMFCAPP_H

#include <QApplication>

#if defined(_AFXDLL) && defined(_MSC_VER)
#define QTWINMIGRATE_WITHMFC
class CWinApp;
#endif

#if defined(Q_OS_WIN)
#  if !defined(QT_QTWINMIGRATE_EXPORT) && !defined(QT_QTWINMIGRATE_IMPORT)
#    define QT_QTWINMIGRATE_EXPORT
#  elif defined(QT_QTWINMIGRATE_IMPORT)
#    if defined(QT_QTWINMIGRATE_EXPORT)
#      undef QT_QTWINMIGRATE_EXPORT
#    endif
#    define QT_QTWINMIGRATE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTWINMIGRATE_EXPORT)
#    undef QT_QTWINMIGRATE_EXPORT
#    define QT_QTWINMIGRATE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTWINMIGRATE_EXPORT
#endif

#if QT_VERSION >= 0x050000
#include <QAbstractNativeEventFilter>

class QT_QTWINMIGRATE_EXPORT QMfcAppEventFilter : public QAbstractNativeEventFilter
{
public:
    QMfcAppEventFilter();
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};
#endif

class QT_QTWINMIGRATE_EXPORT QMfcApp : public QApplication
{
public:
    static bool pluginInstance(Qt::HANDLE plugin = 0);

#ifdef QTWINMIGRATE_WITHMFC
    static int run(CWinApp *mfcApp);
    static QApplication *instance(CWinApp *mfcApp);
    QMfcApp(CWinApp *mfcApp, int &argc, char **argv);
#endif
    QMfcApp(int &argc, char **argv);
    ~QMfcApp();

    bool winEventFilter(MSG *msg, long *result);

    static void enterModalLoop();
    static void exitModalLoop();

private:
#ifdef QTWINMIGRATE_WITHMFC
    static char ** mfc_argv;
    static int mfc_argc;
    static CWinApp *mfc_app;
#endif

    int idleCount;
    bool doIdle;
};

#endif // QMFCAPP_H
