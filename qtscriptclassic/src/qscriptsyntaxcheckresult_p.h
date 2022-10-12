// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTSYNTAXCHECKRESULT_P_H
#define QSCRIPTSYNTAXCHECKRESULT_P_H

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

#if defined(Q_OS_VXWORKS) && defined(m_type)
#  undef m_type
#endif


#include <qatomic.h>
#include <qstring.h>

QT_BEGIN_NAMESPACE

class QScriptSyntaxCheckResultPrivate
{
public:
    QScriptSyntaxCheckResultPrivate() { ref = 0; }
    ~QScriptSyntaxCheckResultPrivate() {}

    QScriptSyntaxCheckResult::State state;
    int errorColumnNumber;
    int errorLineNumber;
    QString errorMessage;
    QBasicAtomicInt ref;
};

QT_END_NAMESPACE


#endif
