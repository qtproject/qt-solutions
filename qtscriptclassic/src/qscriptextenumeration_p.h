// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTEXTENUMERATION_P_H
#define QSCRIPTEXTENUMERATION_P_H

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

#include "qscriptecmacore_p.h"

QT_BEGIN_NAMESPACE


class QScriptValueIteratorImpl;

namespace QScript { namespace Ext {

class EnumerationClassData: public QScriptClassData
{
    QScriptClassInfo *m_classInfo;

public:
    EnumerationClassData(QScriptClassInfo *classInfo);
    virtual ~EnumerationClassData();

    inline QScriptClassInfo *classInfo() const
        { return m_classInfo; }

    virtual void mark(const QScriptValueImpl &object, int generation);
};

class Enumeration: public QScript::Ecma::Core
{
public:
    Enumeration(QScriptEnginePrivate *engine);
    virtual ~Enumeration();

    virtual void execute(QScriptContextPrivate *context);

    class Instance: public QScriptObjectData {
    public:
        Instance() : it(0) {}
        virtual ~Instance();

        static Instance *get(const QScriptValueImpl &object,
                             QScriptClassInfo *klass);

        void toFront();
        void hasNext(QScriptContextPrivate *context, QScriptValueImpl *result);
        void next(QScriptContextPrivate *context, QScriptValueImpl *result);

    public: // attributes
        QScriptValueIteratorImpl *it;
        QScriptValueImpl object;
    };

    void newEnumeration(QScriptValueImpl *result, const QScriptValueImpl &value);

    inline Instance *get(const QScriptValueImpl &object) const
    {
        return Instance::get(object, classInfo());
    }

protected:
    static QScriptValueImpl method_toFront(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
    static QScriptValueImpl method_hasNext(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
    static QScriptValueImpl method_next(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                        QScriptClassInfo *classInfo);
};

} } // namespace QScript::Ext

QT_END_NAMESPACE

#endif
