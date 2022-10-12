// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include "qscriptclassdata_p.h"


QT_BEGIN_NAMESPACE

QScriptClassData::QScriptClassData()
{
}

QScriptClassData::~QScriptClassData()
{
}

void QScriptClassData::mark(const QScriptValueImpl &, int)
{
}

bool QScriptClassData:: resolve(const QScriptValueImpl &, QScriptNameIdImpl *,
                                QScript::Member *, QScriptValueImpl *,
                                QScript::AccessMode)
{
    return false;
}

bool QScriptClassData::get(const QScriptValueImpl &, const QScript::Member &,
                           QScriptValueImpl *)
{
    Q_ASSERT_X(false, "QScriptClassData::get()",
               "implement if resolveMember is implemented");
    return false;
}

bool QScriptClassData::put(QScriptValueImpl *, const QScript::Member &,
                           const QScriptValueImpl &)
{
    Q_ASSERT_X(false, "QScriptClassData::put()",
               "implement if resolveMember is implemented");
    return false;
}

bool QScriptClassData::removeMember(const QScriptValueImpl &,
                                    const QScript::Member &)
{
    return true;
}

bool QScriptClassData::implementsHasInstance(const QScriptValueImpl &)
{
    return false;
}

bool QScriptClassData::hasInstance(const QScriptValueImpl &,
                                   const QScriptValueImpl &)
{
    Q_ASSERT_X(false, "QScriptClassData::hasInstance()",
               "implement if implementsHasInstance() returns true");
    return false;
}

QScriptClassDataIterator *QScriptClassData::newIterator(const QScriptValueImpl &)
{
    return 0;
}

QScriptClassDataIterator::QScriptClassDataIterator()
{
}

QScriptClassDataIterator::~QScriptClassDataIterator()
{
}

QT_END_NAMESPACE

