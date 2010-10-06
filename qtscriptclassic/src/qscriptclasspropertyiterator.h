/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of a Qt Solutions component.
**
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
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
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
****************************************************************************/


#ifndef QSCRIPTCLASSPROPERTYITERATOR_H
#define QSCRIPTCLASSPROPERTYITERATOR_H

#include <QtCore/qstring.h>


#include "qscriptvalue.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Script)

class QScriptClassPropertyIteratorPrivate;
class Q_SCRIPT_EXPORT QScriptClassPropertyIterator
{
protected:
    QScriptClassPropertyIterator(const QScriptValue &object);

public:
    virtual ~QScriptClassPropertyIterator();

    QScriptValue object() const;

    virtual bool hasNext() const = 0;
    virtual void next() = 0;

    virtual bool hasPrevious() const = 0;
    virtual void previous() = 0;

    virtual void toFront() = 0;
    virtual void toBack() = 0;

    virtual QScriptString name() const = 0;
    virtual uint id() const;
    virtual QScriptValue::PropertyFlags flags() const;

protected:
    QScriptClassPropertyIterator(const QScriptValue &object, QScriptClassPropertyIteratorPrivate &dd);
    QScriptClassPropertyIteratorPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptClassPropertyIterator)
    Q_DISABLE_COPY(QScriptClassPropertyIterator)
};

QT_END_NAMESPACE

QT_END_HEADER


#endif
