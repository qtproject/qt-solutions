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


#ifndef QSCRIPTEXTQOBJECT_P_H
#define QSCRIPTEXTQOBJECT_P_H

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

#ifndef QT_NO_QOBJECT

#include "qscriptecmacore_p.h"


#include "qscriptclassdata_p.h"
#include "qscriptfunction_p.h"
#include "qscriptengine.h"
#include "qscriptmemberfwd_p.h"

#include <QHash>
#include <QPointer>
#include <QObject>
#include <QVariant>
#include <QVarLengthArray>
#include <QVector>

QT_BEGIN_NAMESPACE

namespace QScript {

class QObjectConnectionManager;

class ExtQObject: public Ecma::Core
{
public:
    ExtQObject(QScriptEnginePrivate *engine);
    virtual ~ExtQObject();

    virtual void execute(QScriptContextPrivate *context);

    class Instance: public QScriptObjectData {
    public:
        Instance() : ownership(QScriptEngine::QtOwnership) { }
        virtual void finalize(QScriptEnginePrivate *engine);
        virtual ~Instance() {}

        static Instance *get(const QScriptValueImpl &object, QScriptClassInfo *klass = 0);

    public:
        QPointer<QObject> value;
        QScriptEngine::ValueOwnership ownership;
        QScriptEngine::QObjectWrapOptions options;
    };

    inline Instance *get(const QScriptValueImpl &object) const
        { return Instance::get(object, classInfo()); }

    void newQObject(QScriptValueImpl *result, QObject *value,
                    QScriptEngine::ValueOwnership ownership = QScriptEngine::QtOwnership,
                    const QScriptEngine::QObjectWrapOptions &options = 0);

protected:
    static QScriptValueImpl method_findChild(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *classInfo);
    static QScriptValueImpl method_findChildren(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *classInfo);
    static QScriptValueImpl method_toString(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *classInfo);
};

class QtFunction: public QScriptFunction
{
public:
    QtFunction(const QScriptValueImpl &object, int initialIndex, bool maybeOverloaded)
        : m_object(object), m_initialIndex(initialIndex),
          m_maybeOverloaded(maybeOverloaded)
        { }

    virtual ~QtFunction() { }

    virtual void execute(QScriptContextPrivate *context);

    virtual Type type() const { return QScriptFunction::Qt; }

    virtual QString functionName() const;

    virtual void mark(QScriptEnginePrivate *engine, int generation);

    inline QScriptValueImpl object() const { return m_object; }

    inline QObject *qobject() const {
        if (!m_object.isQObject())
            return 0;
        return m_object.toQObject();
    }

    inline const QMetaObject *metaObject() const {
        if (!m_object.isQObject())
            return 0;
        QObject *qobj = m_object.toQObject();
        if (!qobj)
            return 0;
        return qobj->metaObject();
    }

    int mostGeneralMethod(QMetaMethod *out = 0) const;
    QList<int> overloadedIndexes() const;

    inline int initialIndex() const { return m_initialIndex; }
    inline bool maybeOverloaded() const { return m_maybeOverloaded; }

private:
    QScriptValueImpl m_object;
    int m_initialIndex;
    bool m_maybeOverloaded;
};

class ExtQMetaObject: public Ecma::Core
{
public:
    ExtQMetaObject(QScriptEnginePrivate *engine);
    virtual ~ExtQMetaObject();

    virtual void execute(QScriptContextPrivate *context);

    class Instance: public QScriptFunction {
    public:
        Instance() : value(0) { }
        virtual ~Instance() { }

        static Instance *get(const QScriptValueImpl &object, QScriptClassInfo *klass);

        virtual void execute(QScriptContextPrivate *context);

    public:
        const QMetaObject *value;
        QScriptValueImpl ctor;
        QScriptValueImpl prototype;
    };

    inline Instance *get(const QScriptValueImpl &object) const
        { return Instance::get(object, classInfo()); }

    void newQMetaObject(QScriptValueImpl *result, const QMetaObject *value,
                        const QScriptValueImpl &ctor = QScriptValueImpl());

protected:
    static QScriptValueImpl method_className(QScriptContextPrivate *context, QScriptEnginePrivate *eng, QScriptClassInfo *classInfo);
};

} // namespace QScript

struct QScriptQObjectWrapperInfo
{
    QScriptQObjectWrapperInfo(const QScriptValueImpl &obj,
                              QScriptEngine::ValueOwnership own,
                              const QScriptEngine::QObjectWrapOptions &opt)
        : object(obj), ownership(own), options(opt) {}

    QScriptValueImpl object;
    QScriptEngine::ValueOwnership ownership;
    QScriptEngine::QObjectWrapOptions options;
};

class QScriptQObjectData // : public QObjectUserData
{
public:
    QScriptQObjectData();
    ~QScriptQObjectData();

    bool addSignalHandler(QObject *sender,
                          int signalIndex,
                          const QScriptValueImpl &receiver,
                          const QScriptValueImpl &slot,
                          const QScriptValueImpl &senderWrapper,
                          Qt::ConnectionType type);
    bool removeSignalHandler(QObject *sender,
                             int signalIndex,
                             const QScriptValueImpl &receiver,
                             const QScriptValueImpl &slot);

    bool findWrapper(QScriptEngine::ValueOwnership ownership,
                     const QScriptEngine::QObjectWrapOptions &options,
                     QScriptValueImpl *out);
    void registerWrapper(const QScriptValueImpl &wrapper,
                         QScriptEngine::ValueOwnership ownership,
                         const QScriptEngine::QObjectWrapOptions &options);

    void mark(int generation);

private:
    QScript::QObjectConnectionManager *m_connectionManager;
    QList<QScriptQObjectWrapperInfo> wrappers;
};

class QScriptMetaType
{
public:
    enum Kind {
        Invalid,
        Variant,
        MetaType,
        Unresolved,
        MetaEnum
    };

    inline QScriptMetaType()
        : m_kind(Invalid), m_typeId(0) { }

    inline Kind kind() const
    { return m_kind; }

    int typeId() const;

    inline bool isValid() const
    { return (m_kind != Invalid); }

    inline bool isVariant() const
    { return (m_kind == Variant); }

    inline bool isMetaType() const
    { return (m_kind == MetaType); }

    inline bool isUnresolved() const
    { return (m_kind == Unresolved); }

    inline bool isMetaEnum() const
    { return (m_kind == MetaEnum); }

    QByteArray name() const;

    inline int enumeratorIndex() const
    { Q_ASSERT(isMetaEnum()); return m_typeId; }

    inline bool operator==(const QScriptMetaType &other) const
    {
        return (m_kind == other.m_kind) && (m_typeId == other.m_typeId);
    }

    static inline QScriptMetaType variant()
    { return QScriptMetaType(Variant); }

    static inline QScriptMetaType metaType(int typeId, const QByteArray &name)
    { return QScriptMetaType(MetaType, typeId, name); }

    static inline QScriptMetaType metaEnum(int enumIndex, const QByteArray &name)
    { return QScriptMetaType(MetaEnum, enumIndex, name); }

    static inline QScriptMetaType unresolved(const QByteArray &name)
    { return QScriptMetaType(Unresolved, /*typeId=*/0, name); }

private:
    inline QScriptMetaType(Kind kind, int typeId = 0, const QByteArray &name = QByteArray())
        : m_kind(kind), m_typeId(typeId), m_name(name) { }

    Kind m_kind;
    int m_typeId;
    QByteArray m_name;
};

class QScriptMetaMethod
{
public:
    inline QScriptMetaMethod()
        : m_firstUnresolvedIndex(-1)
        { }
    inline QScriptMetaMethod(const QByteArray &name, const QVector<QScriptMetaType> &types)
        : m_name(name), m_types(types), m_firstUnresolvedIndex(-1)
    {
        QVector<QScriptMetaType>::const_iterator it;
        for (it = m_types.constBegin(); it != m_types.constEnd(); ++it) {
            if ((*it).kind() == QScriptMetaType::Unresolved) {
                m_firstUnresolvedIndex = it - m_types.constBegin();
                break;
            }
        }
    }
    inline bool isValid() const
    { return !m_types.isEmpty(); }

    QByteArray name() const
    { return m_name; }

    inline QScriptMetaType returnType() const
    { return m_types.at(0); }

    inline int argumentCount() const
    { return m_types.count() - 1; }

    inline QScriptMetaType argumentType(int arg) const
    { return m_types.at(arg + 1); }

    inline bool fullyResolved() const
    { return m_firstUnresolvedIndex == -1; }

    inline bool hasUnresolvedReturnType() const
    { return (m_firstUnresolvedIndex == 0); }

    inline int firstUnresolvedIndex() const
    { return m_firstUnresolvedIndex; }

    inline int count() const
    { return m_types.count(); }

    inline QScriptMetaType type(int index) const
    { return m_types.at(index); }

    inline QVector<QScriptMetaType> types() const
    { return m_types; }

private:
    QByteArray m_name;
    QVector<QScriptMetaType> m_types;
    int m_firstUnresolvedIndex;
};

struct QScriptMetaArguments
{
    int matchDistance;
    int index;
    QScriptMetaMethod method;
    QVarLengthArray<QVariant, 9> args;

    inline QScriptMetaArguments(int dist, int idx, const QScriptMetaMethod &mtd,
                                const QVarLengthArray<QVariant, 9> &as)
        : matchDistance(dist), index(idx), method(mtd), args(as) { }
    inline QScriptMetaArguments()
        : matchDistance(0), index(-1) { }

    inline bool isValid() const
    { return (index != -1); }
};

class QScriptMetaObject
{
public:
    inline QScriptMetaMethod findMethod(int index) const
    {
        return m_methods.value(index);
    }

    inline void registerMethod(int index, const QScriptMetaMethod &method)
    {
        m_methods.insert(index, method);
    }

    inline bool findMember(QScriptNameIdImpl *nameId, QScript::Member *member) const
    {
        QHash<QScriptNameIdImpl*, QScript::Member>::const_iterator it;
        it = m_members.constFind(nameId);
        if (it == m_members.constEnd())
            return false;
        *member = it.value();
        return true;
    }

    inline void registerMember(QScriptNameIdImpl *nameId, const QScript::Member &member)
    {
        m_members.insert(nameId, member);
    }

    inline QList<QScriptNameIdImpl*> registeredMemberNames() const
    {
        return m_members.keys();
    }

    inline QScriptValueImpl findPropertyAccessor(int index) const
    {
        return m_propertyAccessors.value(index);
    }

    inline void registerPropertyAccessor(int index, const QScriptValueImpl &accessor)
    {
        m_propertyAccessors.insert(index, accessor);
    }

    inline QList<QScriptValueImpl> registeredPropertyAccessors() const
    {
        return m_propertyAccessors.values();
    }

    inline int methodLowerBound(int index) const
    {
        return m_methodBounds.value(index, 0);
    }

    inline void setMethodLowerBound(int index, int bound)
    {
        m_methodBounds.insert(index, bound);
    }

private:
    QHash<int, QScriptValueImpl> m_propertyAccessors;
    QHash<int, QScriptMetaMethod> m_methods;
    QHash<int, int> m_methodBounds;
    QHash<QScriptNameIdImpl*, QScript::Member> m_members;
};

#endif // QT_NO_QOBJECT

QT_END_NAMESPACE

#endif // QSCRIPTEXTQOBJECT_P_H
