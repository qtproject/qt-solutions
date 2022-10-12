// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTSTRING_H
#define QSCRIPTSTRING_H

#include <qstring.h>


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Script)

class QScriptEngine;
class QScriptStringPrivate;

class Q_SCRIPT_EXPORT QScriptString
{
public:
    QScriptString();
    QScriptString(const QScriptString &other);
    ~QScriptString();

    QScriptString &operator=(const QScriptString &other);

    bool isValid() const;

    bool operator==(const QScriptString &other) const;
    bool operator!=(const QScriptString &other) const;

    QString toString() const;
    operator QString() const;

private:
    QScriptStringPrivate *d_ptr;

    Q_DECLARE_PRIVATE(QScriptString)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QSCRIPTSTRING_H
