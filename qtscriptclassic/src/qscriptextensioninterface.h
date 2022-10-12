// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTEXTENSIONINTERFACE_H
#define QSCRIPTEXTENSIONINTERFACE_H

#include <qfactoryinterface.h>


#include <qobject.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Script)

class QScriptEngine;

struct Q_SCRIPT_EXPORT QScriptExtensionInterface
    : public QFactoryInterface
{
    virtual void initialize(const QString &key, QScriptEngine *engine) = 0;
};

Q_DECLARE_INTERFACE(QScriptExtensionInterface,
    "com.trolltech.Qt.QScriptExtensionInterface/1.0")

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSCRIPTEXTENSIONINTERFACE_H
