// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTFUNCTION_P_H
#define QSCRIPTFUNCTION_P_H

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

#include "qscriptobjectdata_p.h"


#include "qscriptglobals_p.h"
#include "qscriptnodepool_p.h"

#include <QList>

#ifndef QT_NO_QOBJECT
# include <QPointer>
# include <QMetaMethod>
#endif

QT_BEGIN_NAMESPACE

class QScriptContext;
class QScriptContextPrivate;
class QScriptNameIdImpl;

class QScriptFunction: public QScriptObjectData
{
public:
    enum Type {
        Unknown,
        Script,
        C,
        C2,
        C3,
        Qt,
        QtProperty
    };

    QScriptFunction(int len = 0)
        : length(len)
        { }
    virtual ~QScriptFunction();

    virtual void execute(QScriptContextPrivate *context) = 0;
    virtual QString toString(QScriptContextPrivate *context) const;

    virtual Type type() const { return Unknown; }

    // name of the file the function is defined in
    virtual QString fileName() const;

    virtual QString functionName() const;

    virtual int startLineNumber() const;

    virtual int endLineNumber() const;

    virtual void mark(QScriptEnginePrivate *engine, int generation);

public: // ### private
    int length;
    QList<QScriptNameIdImpl*> formals;
};

namespace QScript {

// public API function
class CFunction: public QScriptFunction
{
public:
    CFunction(QScriptFunctionSignature funPtr, int length)
        : QScriptFunction(length), m_funPtr(funPtr)
        { }

    virtual ~CFunction() { }

    virtual void execute(QScriptContextPrivate *context);

    virtual Type type() const { return QScriptFunction::C; }

    virtual QString functionName() const;

private:
    QScriptFunctionSignature m_funPtr;
};

// internal API function
class C2Function: public QScriptFunction
{
public:
    C2Function(QScriptInternalFunctionSignature funPtr, int length,
               QScriptClassInfo *classInfo, const QString &name)
        : QScriptFunction(length), m_funPtr(funPtr),
          m_classInfo(classInfo), m_name(name)
        { }

    virtual ~C2Function() {}

    virtual void execute(QScriptContextPrivate *context);

    virtual Type type() const { return QScriptFunction::C2; }

    virtual QString functionName() const;

private:
    QScriptInternalFunctionSignature m_funPtr;
    QScriptClassInfo *m_classInfo;
    QString m_name;
};

class C3Function: public QScriptFunction
{
public:
    C3Function(QScriptFunctionWithArgSignature funPtr, void *arg, int length)
        : QScriptFunction(length), m_funPtr(funPtr), m_arg(arg)
        { }

    virtual ~C3Function() { }

    virtual void execute(QScriptContextPrivate *context);

    virtual Type type() const { return QScriptFunction::C3; }

private:
    QScriptFunctionWithArgSignature m_funPtr;
    void *m_arg;
};

namespace AST {
    class FunctionExpression;
}

// implemented in qscriptcontext_p.cpp
class ScriptFunction: public QScriptFunction
{
public:
    ScriptFunction(AST::FunctionExpression *definition, NodePool *astPool):
        m_definition(definition), m_astPool(astPool), m_compiledCode(0) {}

    virtual ~ScriptFunction() {}

    virtual void execute(QScriptContextPrivate *context);

    virtual QString toString(QScriptContextPrivate *context) const;

    virtual Type type() const
    { return QScriptFunction::Script; }

    virtual QString fileName() const;

    virtual QString functionName() const;

    virtual int startLineNumber() const;

    virtual int endLineNumber() const;

private:
    AST::FunctionExpression *m_definition;
    QExplicitlySharedDataPointer<NodePool> m_astPool;
    Code *m_compiledCode;
};

} // namespace QScript

QT_END_NAMESPACE


#endif // QSCRIPTFUNCTION_P_H
