// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTOBJECTDATA_P_H
#define QSCRIPTOBJECTDATA_P_H

#include <qglobal.h>


QT_BEGIN_NAMESPACE

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

class QScriptEnginePrivate;

class QScriptObjectData
{
protected:
    inline QScriptObjectData() {}

public:
    virtual void finalize(QScriptEnginePrivate *) {}
    virtual ~QScriptObjectData() {}

private:
    Q_DISABLE_COPY(QScriptObjectData)
};

QT_END_NAMESPACE


#endif
