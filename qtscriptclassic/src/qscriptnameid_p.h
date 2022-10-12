// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTNAMEID_P_H
#define QSCRIPTNAMEID_P_H

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

class QScriptNameIdImpl
{
public:
    QString s;
    uint h;
    QScriptNameIdImpl *next;
    uint used: 1;
    uint persistent: 1;
    uint unique: 1;
    uint pad: 29;

    inline QScriptNameIdImpl(const QString &_s):
        s(_s), h(0), next(0), used(0), persistent(0), unique(0), pad(0) { }
};

QT_END_NAMESPACE

#endif // QSCRIPTNAMEID_P_H
