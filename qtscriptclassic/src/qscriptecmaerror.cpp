/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: https://www1.qt.io/terms-conditions
**
** This file is part of the Qt Solutions component.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "qscriptecmaerror_p.h"


#include "qscriptengine_p.h"
#include "qscriptvalueimpl_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"

#include <QtDebug>

QT_BEGIN_NAMESPACE

namespace QScript { namespace Ecma {

static QString getMessage(QScriptContextPrivate *context)
{
    if (context->argumentCount() > 0)
        return context->argument(0).toString();
    return QString();
}

static void setDebugInformation(QScriptValueImpl *error, QScriptContextPrivate *context)
{
    Q_ASSERT(context->previous);    
    context->previous->setDebugInformation(error);
}

static QScriptValueImpl method_EvalError(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl result;
    if (context->isCalledAsConstructor())
        result = context->thisObject();
    eng->errorConstructor->newEvalError(&result, getMessage(context));
    setDebugInformation(&result, context);
    return result;
}

static QScriptValueImpl method_RangeError(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl result;
    if (context->isCalledAsConstructor())
        result = context->thisObject();
    eng->errorConstructor->newRangeError(&result, getMessage(context));
    setDebugInformation(&result, context);
    return result;
}

static QScriptValueImpl method_ReferenceError(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl result;
    if (context->isCalledAsConstructor())
        result = context->thisObject();
    eng->errorConstructor->newReferenceError(&result, getMessage(context));
    setDebugInformation(&result, context);
    return result;
}

static QScriptValueImpl method_SyntaxError(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl result;
    if (context->isCalledAsConstructor())
        result = context->thisObject();
    eng->errorConstructor->newSyntaxError(&result, getMessage(context));
    setDebugInformation(&result, context);
    return result;
}

static QScriptValueImpl method_TypeError(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl result;
    if (context->isCalledAsConstructor())
        result = context->thisObject();
    eng->errorConstructor->newTypeError(&result, getMessage(context));
    setDebugInformation(&result, context);
    return result;
}

static QScriptValueImpl method_UriError(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl result;
    if (context->isCalledAsConstructor())
        result = context->thisObject();
    eng->errorConstructor->newURIError(&result, getMessage(context));
    setDebugInformation(&result, context);
    return result;
}

Error::Error(QScriptEnginePrivate *eng):
    Core(eng, QLatin1String("Error"), QScriptClassInfo::ErrorType)
{
    eng->newFunction(&ctor, this);
    newErrorPrototype(&publicPrototype, QScriptValueImpl(), ctor, QLatin1String("Error"));
    addPrototypeFunction(QLatin1String("backtrace"), method_backtrace, 0);
    addPrototypeFunction(QLatin1String("toString"), method_toString, 0);

    // native errors

    evalErrorCtor = eng->createFunction(method_EvalError, 3,
                                        classInfo(), QLatin1String("EvalError"));
    rangeErrorCtor = eng->createFunction(method_RangeError, 3,
                                         classInfo(), QLatin1String("RangeError"));
    referenceErrorCtor = eng->createFunction(method_ReferenceError, 3,
                                             classInfo(), QLatin1String("ReferenceError"));
    syntaxErrorCtor = eng->createFunction(method_SyntaxError, 3,
                                          classInfo(), QLatin1String("SyntaxError"));
    typeErrorCtor = eng->createFunction(method_TypeError, 3,
                                        classInfo(), QLatin1String("TypeError"));
    uriErrorCtor = eng->createFunction(method_UriError, 3,
                                       classInfo(), QLatin1String("URIError"));

    newErrorPrototype(&evalErrorPrototype, publicPrototype,
                      evalErrorCtor, QLatin1String("EvalError"));
    newErrorPrototype(&rangeErrorPrototype, publicPrototype,
                      rangeErrorCtor, QLatin1String("RangeError"));
    newErrorPrototype(&referenceErrorPrototype, publicPrototype,
                      referenceErrorCtor, QLatin1String("ReferenceError"));
    newErrorPrototype(&syntaxErrorPrototype, publicPrototype,
                      syntaxErrorCtor, QLatin1String("SyntaxError"));
    newErrorPrototype(&typeErrorPrototype, publicPrototype,
                      typeErrorCtor, QLatin1String("TypeError"));
    newErrorPrototype(&uriErrorPrototype, publicPrototype,
                      uriErrorCtor, QLatin1String("URIError"));
}

Error::~Error()
{
}

void Error::execute(QScriptContextPrivate *context)
{
#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    engine()->notifyFunctionEntry(context);
#endif
    QString message = QString();

    if (context->argumentCount() > 0)
        message = context->argument(0).toString();

    QScriptValueImpl result;
    newError(&result, publicPrototype, message);

    setDebugInformation(&result, context);

    context->setReturnValue(result);
#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    engine()->notifyFunctionExit(context);
#endif
}

void Error::mark(QScriptEnginePrivate *eng, int generation)
{
    Core::mark(eng, generation);

    eng->markObject(evalErrorCtor, generation);
    eng->markObject(rangeErrorCtor, generation);
    eng->markObject(referenceErrorCtor, generation);
    eng->markObject(syntaxErrorCtor, generation);
    eng->markObject(typeErrorCtor, generation);
    eng->markObject(uriErrorCtor, generation);

    eng->markObject(evalErrorPrototype, generation);
    eng->markObject(rangeErrorPrototype, generation);
    eng->markObject(referenceErrorPrototype, generation);
    eng->markObject(syntaxErrorPrototype, generation);
    eng->markObject(typeErrorPrototype, generation);
    eng->markObject(uriErrorPrototype, generation);
}

void Error::newError(QScriptValueImpl *result, const QString &message)
{
    newError(result, publicPrototype, message);
}

void Error::newEvalError(QScriptValueImpl *result, const QString &message)
{
    newError(result, evalErrorPrototype, message);
}

void Error::newRangeError(QScriptValueImpl *result, const QString &message)
{
    newError(result, rangeErrorPrototype, message);
}

void Error::newReferenceError(QScriptValueImpl *result, const QString &message)
{
    newError(result, referenceErrorPrototype, message);
}

void Error::newSyntaxError(QScriptValueImpl *result, const QString &message)
{
    newError(result, syntaxErrorPrototype, message);
}

void Error::newTypeError(QScriptValueImpl *result, const QString &message)
{
    newError(result, typeErrorPrototype, message);
}

void Error::newURIError(QScriptValueImpl *result, const QString &message)
{
    newError(result, uriErrorPrototype, message);
}

void Error::newError(QScriptValueImpl *result, const QScriptValueImpl &proto,
                     const QString &message)
{
    QScriptEnginePrivate *eng_p = engine();

    if (!result->isValid())
        eng_p->newObject(result, proto, classInfo());
    else
        result->setClassInfo(classInfo());
    result->setProperty(QLatin1String("message"), QScriptValueImpl(eng_p, message));
}

void Error::newErrorPrototype(QScriptValueImpl *result, const QScriptValueImpl &proto,
                              QScriptValueImpl &ztor, const QString &name)
{
    newError(result, proto);
    result->setProperty(QLatin1String("name"), QScriptValueImpl(engine(), name));
    result->setProperty(QLatin1String("constructor"), ztor,
                        QScriptValue::Undeletable
                        | QScriptValue::SkipInEnumeration);
    ztor.setProperty(QLatin1String("prototype"), *result,
                     QScriptValue::Undeletable
                     | QScriptValue::ReadOnly
                     | QScriptValue::SkipInEnumeration);
}

bool Error::isEvalError(const QScriptValueImpl &value) const
{
    return value.instanceOf(evalErrorPrototype);
}

bool Error::isRangeError(const QScriptValueImpl &value) const
{
    return value.instanceOf(rangeErrorPrototype);
}

bool Error::isReferenceError(const QScriptValueImpl &value) const
{
    return value.instanceOf(referenceErrorPrototype);
}

bool Error::isSyntaxError(const QScriptValueImpl &value) const
{
    return value.instanceOf(syntaxErrorPrototype);
}

bool Error::isTypeError(const QScriptValueImpl &value) const
{
    return value.instanceOf(typeErrorPrototype);
}

bool Error::isURIError(const QScriptValueImpl &value) const
{
    return value.instanceOf(uriErrorPrototype);
}

QStringList Error::backtrace(const QScriptValueImpl &error)
{
    QStringList result;
    QScriptValueImpl stack = error.property(QLatin1String("stack"));
    int frameCount = stack.property(QLatin1String("length")).toInt32();
    for (int i = 0; i < frameCount; ++i) {
        QScriptValueImpl o = stack.property(i);
        QScriptValueImpl frame = o.property(QLatin1String("frame"));
        QString s;
        QString functionName = o.property(QLatin1String("functionName")).toString();
        if (functionName.isEmpty()) {
            if (i == frameCount-1)
                s += QLatin1String("<global>");
            else
                s += QLatin1String("<anonymous>");
        } else {
            s += functionName;
        }
        s += QLatin1Char('(');
        QScriptValueImpl arguments = frame.property(QLatin1String("arguments"));
        if (arguments.isObject()) {
            int argCount = arguments.property(QLatin1String("length")).toInt32();
            for (int j = 0; j < argCount; ++j) {
                if (j > 0)
                    s += QLatin1Char(',');
                s += arguments.property(j).toString();
            }
        }
        s += QLatin1String(")@") + o.property(QLatin1String("fileName")).toString()
             + QLatin1Char(':') + o.property(QLatin1String("lineNumber")).toString();
        result.append(s);
    }
    return result;
}

QScriptValueImpl Error::method_toString(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl name = context->thisObject().property(QLatin1String("name"),
                                                           QScriptValue::ResolvePrototype);
    QScriptValueImpl message = context->thisObject().property(QLatin1String("message"),
                                                              QScriptValue::ResolvePrototype);
    QString result = QLatin1String("");
    if (name.isValid())
        result = name.toString();
    if (message.isValid()) {
        QString str = message.toString();
        if (!str.isEmpty()) {
            if (!result.isEmpty())
                result += QLatin1String(": ");
            result += str;
        }
    }
    return (QScriptValueImpl(eng, result));
}

QScriptValueImpl Error::method_backtrace(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *)
{
    QScriptValueImpl self = context->thisObject();
    return eng->arrayFromStringList(backtrace(self));
}

} } // namespace QSA::Ecma

QT_END_NAMESPACE

