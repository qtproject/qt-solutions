// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTGLOBALS_P_H
#define QSCRIPTGLOBALS_P_H

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

#include <qglobal.h>


QT_BEGIN_NAMESPACE

class QScriptValue;
class QScriptValueImpl;
class QScriptClassInfo;
class QScriptEngine;
class QScriptEnginePrivate;
class QScriptContext;
class QScriptContextPrivate;

typedef QScriptValueImpl (*QScriptInternalFunctionSignature)(QScriptContextPrivate *, QScriptEnginePrivate *, QScriptClassInfo *);
typedef QScriptValue (*QScriptFunctionSignature)(QScriptContext *, QScriptEngine *);
typedef QScriptValue (*QScriptFunctionWithArgSignature)(QScriptContext *, QScriptEngine *, void *);

namespace QScript {

enum Type {
    InvalidType,
    // standard types
    UndefinedType,
    NullType,
    BooleanType,
    StringType,
    NumberType,
    ObjectType,
    // internal types
    IntegerType,
    ReferenceType,
    PointerType,
    LazyStringType
};

enum AccessMode {
    Read = 0x01,
    Write = 0x02,
    ReadWrite = 0x03
};

} // namespace QScript

QT_END_NAMESPACE


#endif // QSCRIPTGLOBALS_P_H
