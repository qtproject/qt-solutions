// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTECMACORE_P_H
#define QSCRIPTECMACORE_P_H

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

#include "qscriptfunction_p.h"


#include "qscriptvalueimplfwd_p.h"
#include "qscriptclassinfo_p.h"

QT_BEGIN_NAMESPACE

namespace QScript { namespace Ecma {

class Core: public QScriptFunction
{
public:
    Core(QScriptEnginePrivate *engine, const QString &className,
         QScriptClassInfo::Type type);
    Core(QScriptEnginePrivate *engine, QScriptClassInfo *classInfo);
    virtual ~Core();

    inline QScriptEnginePrivate *engine() const
    { return m_engine; }

    inline QScriptClassInfo *classInfo() const
    { return m_classInfo; }

    void addPrototypeFunction(
        const QString &name, QScriptInternalFunctionSignature fun, int length,
        const QScriptValue::PropertyFlags flags = QScriptValue::SkipInEnumeration);
    void addConstructorFunction(
        const QString &name, QScriptInternalFunctionSignature fun, int length,
        const QScriptValue::PropertyFlags flags = QScriptValue::SkipInEnumeration);

    QString functionName() const;

    virtual void mark(QScriptEnginePrivate *eng, int generation);

public: // attributes
    QScriptValueImpl ctor;
    QScriptValueImpl publicPrototype;

protected:
    static QScriptValueImpl throwThisObjectTypeError(
        QScriptContextPrivate *context, const QString &functionName);

private:
    void addFunction(QScriptValueImpl &object, const QString &name,
                     QScriptInternalFunctionSignature fun, int length,
                     const QScriptValue::PropertyFlags flags);

    QScriptEnginePrivate *m_engine;
    QScriptClassInfo *m_classInfo;
};

} } // namespace QScript::Ecma


QT_END_NAMESPACE

#endif
