// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTENGINEAGENT_P_H
#define QSCRIPTENGINEAGENT_P_H

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

class QScriptEngineAgent;
class Q_SCRIPT_EXPORT QScriptEngineAgentPrivate
{
    Q_DECLARE_PUBLIC(QScriptEngineAgent)
public:
    QScriptEngineAgentPrivate();
    virtual ~QScriptEngineAgentPrivate();

    QScriptEngine *engine;

    QScriptEngineAgent *q_ptr;
};

QT_END_NAMESPACE


#endif
