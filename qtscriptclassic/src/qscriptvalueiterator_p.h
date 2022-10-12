// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTVALUEITERATOR_P_H
#define QSCRIPTVALUEITERATOR_P_H

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

class QScriptValueIteratorImpl;

class QScriptValueIterator;
class QScriptValueIteratorPrivate
{
    Q_DECLARE_PUBLIC(QScriptValueIterator)
public:
    QScriptValueIteratorPrivate();
    ~QScriptValueIteratorPrivate();

    QScriptValueIterator *q_ptr;

    QScriptValueIteratorImpl *it;
};

QT_END_NAMESPACE

#endif // QSCRIPTVALUEITERATOR_P_H
