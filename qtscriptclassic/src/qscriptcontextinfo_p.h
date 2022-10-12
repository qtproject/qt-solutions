// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTCONTEXTINFO_P_H
#define QSCRIPTCONTEXTINFO_P_H

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

#include "qscriptcontextinfo.h"


#include <qatomic.h>
#include <qstring.h>

QT_BEGIN_NAMESPACE

class QScriptContext;

class QScriptContextInfo;
class QScriptContextInfoPrivate
{
    Q_DECLARE_PUBLIC(QScriptContextInfo)
public:
    QScriptContextInfoPrivate();
    QScriptContextInfoPrivate(const QScriptContext *context);
    ~QScriptContextInfoPrivate();

    qint64 scriptId;
    int lineNumber;
    int columnNumber;
    QString fileName;

    QString functionName;
    QScriptContextInfo::FunctionType functionType;

    int functionStartLineNumber;
    int functionEndLineNumber;
    int functionMetaIndex;

    QStringList parameterNames;

    QBasicAtomicInt ref;

    QScriptContextInfo *q_ptr;
};

QT_END_NAMESPACE


#endif
