// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTECMABOOLEAN_P_H
#define QSCRIPTECMABOOLEAN_P_H

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

class Boolean: public Core
{
public:
    Boolean(QScriptEnginePrivate *engine);
    virtual ~Boolean();

    virtual void execute(QScriptContextPrivate *context);

    void newBoolean(QScriptValueImpl *result, bool value = false);

protected:
    static QScriptValueImpl method_toString(QScriptContextPrivate *context,
                                            QScriptEnginePrivate *eng,
                                            QScriptClassInfo *classInfo);
    static QScriptValueImpl method_valueOf(QScriptContextPrivate *context,
                                           QScriptEnginePrivate *eng,
                                           QScriptClassInfo *classInfo);
};

} } // namespace QScript::Ecma


QT_END_NAMESPACE

#endif
