// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTMEMBER_P_H
#define QSCRIPTMEMBER_P_H

#include "qscriptmemberfwd_p.h"


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

inline void QScript::Member::resetFlags(uint flags)
{
    m_flags = flags;
}

inline void QScript::Member::setFlags(uint flags)
{
    m_flags |= flags;
}

inline void QScript::Member::unsetFlags(uint flags)
{
    m_flags &= ~flags;
}

inline uint QScript::Member::flags() const
{
    return m_flags;
}

inline bool QScript::Member::testFlags(uint mask) const
{
    return m_flags & mask;
}

inline bool QScript::Member::isValid() const
{
    return m_flags & 0x00000300;
}

inline bool QScript::Member::isWritable() const
{
    return !(m_flags & QScriptValue::ReadOnly);
}

inline bool QScript::Member::isDeletable() const
{
    return !(m_flags & QScriptValue::Undeletable);
}

inline bool QScript::Member::dontEnum() const
{
    return m_flags & QScriptValue::SkipInEnumeration;
}

inline bool QScript::Member::isObjectProperty() const
{
    return m_flags & ObjectProperty;
}

inline bool QScript::Member::isNativeProperty() const
{
    return m_flags & NativeProperty;
}

inline bool QScript::Member::isUninitializedConst() const
{
    return m_flags & UninitializedConst;
}

inline bool QScript::Member::isGetter() const
{
    return m_flags & QScriptValue::PropertyGetter;
}

inline bool QScript::Member::isSetter() const
{
    return m_flags & QScriptValue::PropertySetter;
}

inline bool QScript::Member::isGetterOrSetter() const
{
    return m_flags & (QScriptValue::PropertyGetter | QScriptValue::PropertySetter);
}

inline int QScript::Member::id() const
{
    return m_id;
}

inline QScriptNameIdImpl *QScript::Member::nameId() const
{
    return m_nameId;
}

inline QScript::Member QScript::Member::invalid()
{
    Member m;
    m.m_flags = 0;
    return m;
}

inline void QScript::Member::invalidate()
{
    m_flags = 0;
}

inline void QScript::Member::native(QScriptNameIdImpl *nameId, int id, uint flags)
{
    Q_ASSERT(! (flags & ObjectProperty));

    m_nameId = nameId;
    m_id = id;
    m_flags = flags | NativeProperty;
}

inline void QScript::Member::object(QScriptNameIdImpl *nameId, int id, uint flags)
{
    Q_ASSERT(! (flags & NativeProperty));

    m_nameId = nameId;
    m_id = id;
    m_flags = flags | ObjectProperty;
}

inline bool QScript::Member::operator==(const QScript::Member &other) const
{
    return m_nameId == other.m_nameId;
}

inline bool QScript::Member::operator!=(const QScript::Member &other) const
{
    return m_nameId != other.m_nameId;
}

QT_END_NAMESPACE


#endif
