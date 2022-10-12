// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include "qscriptecmaboolean_p.h"


#include "qscriptengine_p.h"
#include "qscriptvalueimpl_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"

#include <QtDebug>

QT_BEGIN_NAMESPACE

namespace QScript { namespace Ecma {

Boolean::Boolean(QScriptEnginePrivate *eng):
    Core(eng, QLatin1String("Boolean"), QScriptClassInfo::BooleanType)
{
    newBoolean(&publicPrototype, false);

    eng->newConstructor(&ctor, this, publicPrototype);

    addPrototypeFunction(QLatin1String("toString"), method_toString, 0);
    addPrototypeFunction(QLatin1String("valueOf"), method_valueOf, 0);
}

Boolean::~Boolean()
{
}

void Boolean::execute(QScriptContextPrivate *context)
{
#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    engine()->notifyFunctionEntry(context);
#endif
    bool value;
    if (context->argumentCount() > 0)
        value = context->argument(0).toBoolean();
    else
        value = false;

    QScriptValueImpl boolean(value);
    if (!context->isCalledAsConstructor()) {
        context->setReturnValue(boolean);
    } else {
        QScriptValueImpl &obj = context->m_thisObject;
        obj.setClassInfo(classInfo());
        obj.setInternalValue(boolean);
        obj.setPrototype(publicPrototype);
        context->setReturnValue(obj);
    }
#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    engine()->notifyFunctionExit(context);
#endif
}

void Boolean::newBoolean(QScriptValueImpl *result, bool value)
{
    engine()->newObject(result, publicPrototype, classInfo());
    result->setInternalValue(QScriptValueImpl(value));
}

QScriptValueImpl Boolean::method_toString(QScriptContextPrivate *context,
                                          QScriptEnginePrivate *eng,
                                          QScriptClassInfo *classInfo)
{
    QScriptValueImpl self = context->thisObject();
    if (self.classInfo() != classInfo) {
        return throwThisObjectTypeError(
            context, QLatin1String("Boolean.prototype.toString"));
    }
    const QScript::IdTable *t = eng->idTable();
    bool v = self.internalValue().toBoolean();
    QScriptValueImpl result;
    eng->newNameId(&result, v ? t->id_true : t->id_false);
    return result;
}

QScriptValueImpl Boolean::method_valueOf(QScriptContextPrivate *context,
                                         QScriptEnginePrivate *,
                                         QScriptClassInfo *classInfo)
{
    QScriptValueImpl self = context->thisObject();
    if (self.classInfo() != classInfo) {
        return throwThisObjectTypeError(
            context, QLatin1String("Boolean.prototype.valueOf"));
    }
    return self.internalValue();
}

} } // namespace QScript::Ecma

QT_END_NAMESPACE

