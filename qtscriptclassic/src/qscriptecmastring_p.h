// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTECMASTRING_P_H
#define QSCRIPTECMASTRING_P_H

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

namespace QScript { namespace Ecma {

class String: public Core
{
public:
    String(QScriptEnginePrivate *engine);
    virtual ~String();

    virtual void execute(QScriptContextPrivate *context);

    void newString(QScriptValueImpl *result, const QString &value = QString());

protected:
    static QScriptValueImpl method_toString(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                        QScriptClassInfo *classInfo);
    static QScriptValueImpl method_valueOf(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                       QScriptClassInfo *classInfo);
    static QScriptValueImpl method_charAt(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                      QScriptClassInfo *classInfo);
    static QScriptValueImpl method_charCodeAt(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                          QScriptClassInfo *classInfo);
    static QScriptValueImpl method_concat(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                      QScriptClassInfo *classInfo);
    static QScriptValueImpl method_indexOf(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                       QScriptClassInfo *classInfo);
    static QScriptValueImpl method_lastIndexOf(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
    static QScriptValueImpl method_localeCompare(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                             QScriptClassInfo *classInfo);
    static QScriptValueImpl method_match(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                     QScriptClassInfo *classInfo);
    static QScriptValueImpl method_replace(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                       QScriptClassInfo *classInfo);
    static QScriptValueImpl method_search(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                      QScriptClassInfo *classInfo);
    static QScriptValueImpl method_slice(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                     QScriptClassInfo *classInfo);
    static QScriptValueImpl method_split(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                     QScriptClassInfo *classInfo);
    static QScriptValueImpl method_substr(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                          QScriptClassInfo *classInfo);
    static QScriptValueImpl method_substring(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                         QScriptClassInfo *classInfo);
    static QScriptValueImpl method_toLowerCase(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
    static QScriptValueImpl method_toLocaleLowerCase(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                                 QScriptClassInfo *classInfo);
    static QScriptValueImpl method_toUpperCase(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
    static QScriptValueImpl method_toLocaleUpperCase(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                                 QScriptClassInfo *classInfo);
    static QScriptValueImpl method_fromCharCode(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                            QScriptClassInfo *classInfo);

public:
    // Qt extensions
    static QScriptValueImpl method_ext_arg(QScriptContextPrivate *context, QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
};

} } // namespace QScript::Ecma

QT_END_NAMESPACE


#endif
