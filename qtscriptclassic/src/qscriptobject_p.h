// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTOBJECT_P_H
#define QSCRIPTOBJECT_P_H

#include "qscriptobjectfwd_p.h"


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

inline bool QScriptObject::findMember(QScriptNameIdImpl *nameId,
                       QScript::Member *m) const
{
    const QScript::Member *members = m_members.constData();
    const int size = m_members.size();

    const QScript::Member *first = &members[-1];
    const QScript::Member *last = &members[size - 1];

    for (const QScript::Member *it = last; it != first; --it) {
        if (it->nameId() == nameId && it->isValid()) {
            *m = *it;
            return true;
        }
    }

    return false;
}

// assumes that m already points to the setter
inline bool QScriptObject::findGetter(QScript::Member *m) const
{
    const QScript::Member *members = m_members.constData();
    const QScript::Member *first = &members[-1];
    const QScript::Member *last = &members[m->id() - 1];

    for (const QScript::Member *it = last; it != first; --it) {
        if (it->nameId() == m->nameId() && it->isValid() && it->isGetter()) {
            *m = *it;
            return true;
        }
    }

    return false;
}

// assumes that m already points to the getter
inline bool QScriptObject::findSetter(QScript::Member *m) const
{
    const QScript::Member *members = m_members.constData();
    const QScript::Member *first = &members[-1];
    const QScript::Member *last = &members[m->id() - 1];

    for (const QScript::Member *it = last; it != first; --it) {
        if (it->nameId() == m->nameId() && it->isValid() && it->isSetter()) {
            *m = *it;
            return true;
        }
    }

    return false;
}

inline int QScriptObject::memberCount() const
{
    return m_members.size();
}

inline void QScriptObject::createMember(QScriptNameIdImpl *nameId,
                         QScript::Member *member, uint flags)
{
    member->object(nameId, m_values.size(), flags);
    m_members.append(*member);
    m_values.append(QScriptValueImpl());
}

inline void QScriptObject::member(int index, QScript::Member *member)
{
    *member = m_members[index];
}

inline void QScriptObject::put(const QScript::Member &m, const QScriptValueImpl &v)
{
    m_values[m.id()] = v;
}

inline QScriptValueImpl &QScriptObject::reference(const QScript::Member &m)
{
    return m_values[m.id()];
}

inline void QScriptObject::get(const QScript::Member &m, QScriptValueImpl *v)
{
    Q_ASSERT(m.isObjectProperty());
    *v = m_values[m.id()];
}

inline void QScriptObject::removeMember(const QScript::Member &member)
{
    m_members[member.id()].invalidate();
    m_values[member.id()].invalidate();
}

inline QScriptObject::~QScriptObject()
{
    finalize();
}

inline void QScriptObject::finalize()
{
    finalizeData();
}

inline void QScriptObject::finalizeData()
{
    if (m_data) {
        m_data->finalize(m_class->engine());
        delete m_data;
        m_data = 0;
    }
}

inline void QScriptObject::reset()
{
    m_prototype.invalidate();
    m_scope.invalidate();
    m_internalValue.invalidate();
    m_members.resize(0);
    m_values.resize(0);
    m_data = 0;
}

QT_END_NAMESPACE


#endif
