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
