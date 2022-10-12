// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTCLASS_P_H
#define QSCRIPTCLASS_P_H

#include <qobjectdefs.h>


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

class QScriptEngine;
class QScriptClassInfo;
class QScriptFunction;

class QScriptClass;
class QScriptClassPrivate
{
    Q_DECLARE_PUBLIC(QScriptClass)
public:
    QScriptClassPrivate(QScriptClass*);
    virtual ~QScriptClassPrivate();

    static QScriptClassPrivate *get(QScriptClass *klass);

    QScriptClassInfo *classInfo();
    static QScriptClass *classFromInfo(QScriptClassInfo *info);

    QScriptFunction *newFunction();

    QScriptEngine *engine;
    QScriptClassInfo *m_classInfo;

    QScriptClass *q_ptr;
};

QT_END_NAMESPACE


#endif
