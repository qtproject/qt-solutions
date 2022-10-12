// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTCLASSPROPERTYITERATOR_P_H
#define QSCRIPTCLASSPROPERTYITERATOR_P_H

#include <qobjectdefs.h>


#include "qscriptvalue.h"

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

class QScriptClassPropertyIterator;
class QScriptClassPropertyIteratorPrivate
{
    Q_DECLARE_PUBLIC(QScriptClassPropertyIterator)
public:
    QScriptClassPropertyIteratorPrivate(QScriptClassPropertyIterator*);
    virtual ~QScriptClassPropertyIteratorPrivate();

    QScriptValue object;

    QScriptClassPropertyIterator *q_ptr;
};

QT_END_NAMESPACE


#endif
