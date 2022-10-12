// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include "qscriptextvariant_p.h"


#include "qscriptengine_p.h"
#include "qscriptvalueimpl_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"

#include <QtDebug>

#include <QStringList>

#include <limits.h>

QT_BEGIN_NAMESPACE

namespace QScript { namespace Ext {

Variant::Variant(QScriptEnginePrivate *eng):
    Ecma::Core(eng, QLatin1String("QVariant"), QScriptClassInfo::VariantType)
{
    newVariant(&publicPrototype, QVariant());

    eng->newConstructor(&ctor, this, publicPrototype);

    addPrototypeFunction(QLatin1String("toString"), method_toString, 0);
    addPrototypeFunction(QLatin1String("valueOf"), method_valueOf, 0);
}

Variant::~Variant()
{
}

Variant::Instance *Variant::Instance::get(const QScriptValueImpl &object, QScriptClassInfo *klass)
{
    if (! klass || klass == object.classInfo())
        return static_cast<Instance*> (object.objectData());

    return 0;
}

void Variant::execute(QScriptContextPrivate *context)
{
    QScriptValueImpl tmp;
    newVariant(&tmp, QVariant());
    context->setReturnValue(tmp);
}

void Variant::newVariant(QScriptValueImpl *result, const QVariant &value)
{
    Instance *instance;
    if (!result->isValid()) {
        engine()->newObject(result, publicPrototype, classInfo());
        instance = new Instance();
        result->setObjectData(instance);
    } else {
        Q_ASSERT(result->isObject());
        if (result->classInfo() != classInfo()) {
            result->destroyObjectData();
            result->setClassInfo(classInfo());
            instance = new Instance();
            result->setObjectData(instance);
        } else {
            instance = Instance::get(*result, classInfo());
        }
    }
    instance->value = value;
}

QScriptValueImpl Variant::method_toString(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *classInfo)
{
    if (Instance *instance = Instance::get(context->thisObject(), classInfo)) {
        QString result;
        QScriptValueImpl value = method_valueOf(context, eng, classInfo);
        if (value.isObject()) {
            result = instance->value.toString();
            if (result.isEmpty()) {
                result = QString::fromLatin1("QVariant(%0)")
                         .arg(QLatin1String(instance->value.typeName()));
            }
        } else {
            result = value.toString();
        }
        return QScriptValueImpl(eng, result);
    }
    return context->throwError(QScriptContext::TypeError,
                               QLatin1String("QVariant.prototype.toString"));
}

QScriptValueImpl Variant::method_valueOf(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *classInfo)
{
    if (Instance *instance = Instance::get(context->thisObject(), classInfo)) {
        QVariant v = instance->value;
        switch (v.type ()) {
        case QVariant::Invalid:
            return eng->undefinedValue();
        case QVariant::String:
            return (QScriptValueImpl(eng, v.toString()));

        case QVariant::Int:
            return (QScriptValueImpl(v.toInt()));

        case QVariant::Bool:
            return (QScriptValueImpl(v.toBool()));

        case QVariant::Double:
            return (QScriptValueImpl(v.toDouble())); // ### hmmm

        case QVariant::Char:
            return (QScriptValueImpl(v.toChar().unicode()));

        case QVariant::UInt:
            return (QScriptValueImpl(v.toUInt()));

        default:
            return context->thisObject();
        } // switch
    }
    return context->thisObject();
}

} } // namespace QScript::Ecma

QT_END_NAMESPACE

