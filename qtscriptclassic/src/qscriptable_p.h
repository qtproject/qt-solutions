// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTABLE_P_H
#define QSCRIPTABLE_P_H

#include <qobjectdefs.h>

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


QT_BEGIN_NAMESPACE

class QScriptEngine;

class QScriptablePrivate
{
    Q_DECLARE_PUBLIC(QScriptable)
public:
    inline QScriptablePrivate()
        : engine(0)
        { }

    static inline QScriptablePrivate *get(QScriptable *q)
        { return q->d_func(); }

    QScriptEngine *engine;

    QScriptable *q_ptr;
};

QT_END_NAMESPACE


#endif
