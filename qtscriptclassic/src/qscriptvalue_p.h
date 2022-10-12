// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTVALUE_P_H
#define QSCRIPTVALUE_P_H

#include "qscriptvaluefwd_p.h"

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

inline QScriptValuePrivate::QScriptValuePrivate()
{
    engine = 0;
    ref = 0;
}

inline QScriptValuePrivate::~QScriptValuePrivate()
{
    if (value.type() == QScript::LazyStringType)
        delete value.m_lazy_string_value;
}

inline QScriptValuePrivate *QScriptValuePrivate::create()
{
    return new QScriptValuePrivate();
}

inline QScriptValuePrivate *QScriptValuePrivate::get(const QScriptValue &value)
{
    return const_cast<QScriptValuePrivate*>(value.d_func());
}

inline QScriptValueImpl QScriptValuePrivate::valueOf(const QScriptValue &value)
{
    QScriptValuePrivate *p = const_cast<QScriptValuePrivate*>(value.d_func());
    if (!p)
        return QScriptValueImpl();
    return p->value;
}

inline void QScriptValuePrivate::init(QScriptValue &value, QScriptValuePrivate *p)
{
    value.d_ptr = p;
    value.d_ptr->ref.ref();
}

inline void QScriptValuePrivate::invalidate()
{
    engine = 0;
    value.invalidate();
}

QT_END_NAMESPACE


#endif
