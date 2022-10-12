// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include "qscriptfunction_p.h"


#include "qscriptengine_p.h"
#include "qscriptvalueimpl_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"

QT_BEGIN_NAMESPACE

QScriptFunction::~QScriptFunction()
{
}

QString QScriptFunction::toString(QScriptContextPrivate *) const
{
    QString result;
    result += QLatin1String("function () { [native] }");
    return result;
}

QString QScriptFunction::fileName() const
{
    return QString();
}

QString QScriptFunction::functionName() const
{
    return QString();
}

int QScriptFunction::startLineNumber() const
{
    return -1;
}

int QScriptFunction::endLineNumber() const
{
    return -1;
}

void QScriptFunction::mark(QScriptEnginePrivate *engine, int generation)
{
    for (int i = 0; i < formals.count(); ++i)
        engine->markString(formals.at(i), generation);
}

// public API function
void QScript::CFunction::execute(QScriptContextPrivate *context)
{
    QScriptEnginePrivate *eng_p = context->engine();

    context->m_result = eng_p->undefinedValue();

#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    eng_p->notifyFunctionEntry(context);
#endif

    QScriptContext *publicContext = QScriptContextPrivate::get(eng_p->currentContext());
    QScriptEngine *publicEngine = QScriptEnginePrivate::get(eng_p);
    QScriptValueImpl result = eng_p->toImpl((*m_funPtr)(publicContext, publicEngine));
    if (result.isValid() && !eng_p->shouldAbort()
        && (context->state() == QScriptContext::NormalState)) {
        context->m_result = result;
    }

#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    eng_p->notifyFunctionExit(context);
#endif
}

QString QScript::CFunction::functionName() const
{
    return QString();
}

// internal API function
void QScript::C2Function::execute(QScriptContextPrivate *context)
{
    QScriptEnginePrivate *eng_p = context->engine();

    bool blocked = eng_p->blockGC(true);

#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    eng_p->notifyFunctionEntry(context);
#endif

    context->m_result = (*m_funPtr)(context, eng_p, m_classInfo);
    Q_ASSERT(context->m_result.isValid());

#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    eng_p->notifyFunctionExit(context);
#endif

    eng_p->blockGC(blocked);
}

QString QScript::C2Function::functionName() const
{
    if (!m_name.isEmpty())
        return m_name;
    return QString();
}

void QScript::C3Function::execute(QScriptContextPrivate *context)
{
    QScriptEnginePrivate *eng_p = context->engine();

    context->m_result = eng_p->undefinedValue();

#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    eng_p->notifyFunctionEntry(context);
#endif

    QScriptContext *publicContext = QScriptContextPrivate::get(eng_p->currentContext());
    QScriptEngine *publicEngine = QScriptEnginePrivate::get(eng_p);
    QScriptValueImpl result = eng_p->toImpl((*m_funPtr)(publicContext, publicEngine, m_arg));
    if (result.isValid() && !eng_p->shouldAbort())
        context->m_result = result;

#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    eng_p->notifyFunctionExit(context);
#endif
}

QT_END_NAMESPACE

