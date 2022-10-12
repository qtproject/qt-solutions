// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTOBJECTFWD_P_H
#define QSCRIPTOBJECTFWD_P_H

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

#include <qglobal.h>


#include "qscriptbuffer_p.h"
#include "qscriptmemberfwd_p.h"
#include "qscriptvalueimplfwd_p.h"

QT_BEGIN_NAMESPACE

class QScriptObjectData;

class QScriptObject
{
public:
    inline void reset();
    inline ~QScriptObject();
    inline void finalize();
    inline void finalizeData();

    inline bool findMember(QScriptNameIdImpl *nameId,
                           QScript::Member *m) const;

    inline bool findGetter(QScript::Member *m) const;

    inline bool findSetter(QScript::Member *m) const;

    inline int memberCount() const;

    inline void createMember(QScriptNameIdImpl *nameId,
                             QScript::Member *member, uint flags);

    inline void member(int index, QScript::Member *member);

    inline void put(const QScript::Member &m, const QScriptValueImpl &v);

    inline QScriptValueImpl &reference(const QScript::Member &m);

    inline void get(const QScript::Member &m, QScriptValueImpl *v);

    inline void removeMember(const QScript::Member &member);

    QScriptValueImpl m_prototype;
    QScriptValueImpl m_scope;
    QScriptValueImpl m_internalValue; // [[value]]
    QScriptObjectData *m_data;
    QScript::Buffer<QScript::Member> m_members;
    QScript::Buffer<QScriptValueImpl> m_values;
    qint64 m_id;
    QScriptClassInfo *m_class;
};

QT_END_NAMESPACE


#endif
