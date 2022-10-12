// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTVALUEFWD_P_H
#define QSCRIPTVALUEFWD_P_H

#include <qatomic.h>


#include "qscriptvalueimplfwd_p.h"

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

class QScriptValuePrivate
{
public:
    inline QScriptValuePrivate();
    inline ~QScriptValuePrivate();

    static inline QScriptValuePrivate *create();

    static inline QScriptValuePrivate *get(const QScriptValue &value);

    static inline QScriptValueImpl valueOf(const QScriptValue &value);

    static inline void init(QScriptValue &value, QScriptValuePrivate *p);

    inline void invalidate();

    QScriptEngine *engine;
    QScriptValueImpl value;
    QBasicAtomicInt ref;
};

QT_END_NAMESPACE


#endif
