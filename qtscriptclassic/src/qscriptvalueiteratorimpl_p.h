// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTVALUEITERATORIMPL_P_H
#define QSCRIPTVALUEITERATORIMPL_P_H

#include "qscriptvalueimplfwd_p.h"


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

class QScriptClassDataIterator;
class QScriptNameIdImpl;

class QScriptValueIteratorImpl
{
public:
    QScriptValueIteratorImpl(const QScriptValueImpl &obj);
    ~QScriptValueIteratorImpl();

    bool ignoresDontEnum() const;
    void setIgnoresDontEnum(bool ignore);

    bool enumeratePrototype() const;
    void setEnumeratePrototype(bool enable);

    bool hasNext();
    void next();

    bool hasPrevious();
    void previous();

    QScript::Member *member();

    QScriptNameIdImpl *nameId() const;
    QString name() const;

    QScriptValueImpl value() const;
    void setValue(const QScriptValueImpl &value);

    uint flags() const;

    void remove();

    void toFront();
    void toBack();

    QScriptValueImpl object() const;
    void setObject(const QScriptValueImpl &obj);

private:
    bool acceptsMember(const QScriptValueImpl &o, const QScript::Member &m) const;
    QScriptClassDataIterator *getClassDataIterator();

    QScriptValueImpl m_frontObject;

    bool m_ignoresDontEnum;
    bool m_enumerateProto;

    QScriptValueImpl m_object;
    QScript::Member m_member;

    int m_searchIndex;
    QScriptValueImpl m_foundObject;
    QScript::Member m_foundMember;
    bool m_foundForward;
    QScriptClassDataIterator *m_classDataIterator;
    bool m_searchClassDataIterator;
};

QT_END_NAMESPACE


#endif
