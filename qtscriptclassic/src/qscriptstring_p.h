// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTSTRING_P_H
#define QSCRIPTSTRING_P_H

#include <qatomic.h>


QT_BEGIN_NAMESPACE

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class QScriptNameIdImpl;
class QScriptEnginePrivate;
class QScriptString;

class QScriptStringPrivate
{
    Q_DECLARE_PUBLIC(QScriptString)
public:
    QScriptStringPrivate();

    static QScriptStringPrivate *create();
    static QScriptStringPrivate *get(const QScriptString &q);
    static void init(QScriptString &q, QScriptStringPrivate *d);

    QBasicAtomicInt ref;
    QScriptNameIdImpl *nameId;
    QScriptEnginePrivate *engine;
    QScriptString *q_ptr;
};

QT_END_NAMESPACE


#endif
