// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTCLASSDATA_P_H
#define QSCRIPTCLASSDATA_P_H

#include "qscriptglobals_p.h"


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

class QScriptValueImpl;
class QScriptNameIdImpl;
class QScriptClassDataIterator;

namespace QScript {
    class Member;
}

class QScriptClassData
{
protected:
    QScriptClassData();

public:
    virtual ~QScriptClassData();

    virtual void mark(const QScriptValueImpl &object, int generation);
    virtual bool resolve(const QScriptValueImpl &object, QScriptNameIdImpl *nameId,
                         QScript::Member *member, QScriptValueImpl *base,
                         QScript::AccessMode access);
    virtual bool get(const QScriptValueImpl &obj, const QScript::Member &m,
                     QScriptValueImpl *result);
    virtual bool put(QScriptValueImpl *object, const QScript::Member &member,
                     const QScriptValueImpl &value);
    virtual bool removeMember(const QScriptValueImpl &object,
                              const QScript::Member &member);
    virtual bool implementsHasInstance(const QScriptValueImpl &object);
    virtual bool hasInstance(const QScriptValueImpl &object,
                             const QScriptValueImpl &value);
    virtual QScriptClassDataIterator *newIterator(const QScriptValueImpl &object);

private:
    Q_DISABLE_COPY(QScriptClassData)
};

class QScriptClassDataIterator
{
protected:
    QScriptClassDataIterator();

public:
    virtual ~QScriptClassDataIterator();

    virtual bool hasNext() const = 0;
    virtual void next(QScript::Member *member) = 0;

    virtual bool hasPrevious() const = 0;
    virtual void previous(QScript::Member *member) = 0;

    virtual void toFront() = 0;
    virtual void toBack() = 0;
};

QT_END_NAMESPACE


#endif // QSCRIPTCLASSDATA_P_H
