// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTCLASSINFO_P_H
#define QSCRIPTCLASSINFO_P_H

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

#if defined(Q_OS_VXWORKS) && defined(m_type)
#  undef m_type
#endif

#include "qscriptclassdata_p.h"


#include <qstring.h>

QT_BEGIN_NAMESPACE

class QScriptClassInfo
{
public:
    enum Type {
        FunctionBased   = 0x40000000,

        ObjectType      = 1,
        FunctionType    = 2 | FunctionBased,
        ArrayType       = 3,
        StringType      = 4,
        BooleanType     = 5,
        NumberType      = 6,
        DateType        = 7,
        RegExpType      = 8,
        ErrorType       = 9,

        VariantType     = 10,
        QObjectType     = 11,
        QMetaObjectType = 12 | FunctionBased,

        // Types used by the runtime
        ActivationType  = 100,
        EnumerationType = 101,

        CustomType      = 1000,

        TypeMask        = 0x0000FFFF
    };

    inline QScriptClassInfo(QScriptEnginePrivate *engine, Type type, const QString &name)
        : m_engine(engine), m_type(type), m_name(name), m_data(0) { }
    inline ~QScriptClassInfo() { delete m_data; }

    inline QScriptEnginePrivate *engine() const
        { return m_engine; }
    inline Type type() const
        { return m_type; }
    inline QString name() const
        { return m_name; }

    inline void setData(QScriptClassData *data)
        { m_data = data; }
    QScriptClassData *data() const
        { return m_data; }

private:
    QScriptEnginePrivate *m_engine;
    Type m_type;
    QString m_name;
    QScriptClassData *m_data;

private:
    Q_DISABLE_COPY(QScriptClassInfo)
};

QT_END_NAMESPACE

#endif // QSCRIPTCLASSINFO_P_H
