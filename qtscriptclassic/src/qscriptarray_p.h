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


#ifndef QSCRIPTARRAY_P_H
#define QSCRIPTARRAY_P_H

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

#include <QMap>


#include <QVector>

#include "qscriptvalueimplfwd_p.h"
#include "qscriptenginefwd_p.h"

QT_BEGIN_NAMESPACE

namespace QScript {

class Array
{
public:
    inline Array(QScriptEnginePrivate *engine);
    inline Array(const Array &other);
    inline ~Array();

    inline Array &operator = (const Array &other);

    inline bool isEmpty() const;
    inline uint size() const;
    inline uint count() const;
    inline QScriptValueImpl at(uint index) const;
    inline void assign(uint index, const QScriptValueImpl &v);
    inline void clear();
    inline void mark(int generation);
    inline void resize(uint size);
    inline void concat(const Array &other);
    inline QScriptValueImpl pop();
    inline void sort(const QScriptValueImpl &comparefn);
    inline void splice(qsreal start, qsreal deleteCount,
                       const QVector<QScriptValueImpl> &items,
                       Array &other);
    inline QList<uint> keys() const;

private:
    enum Mode {
        VectorMode,
        MapMode
    };

    QScriptEnginePrivate *m_engine;
    Mode m_mode;
    int m_instances;

    union {
        QMap<uint, QScriptValueImpl> *to_map;
        QVector<QScriptValueImpl> *to_vector;
    };
};

class ArrayElementLessThan
{
public:
    inline ArrayElementLessThan(const QScriptValueImpl &comparefn)
        : m_comparefn(comparefn) {}

    inline bool operator()(const QScriptValueImpl &v1, const QScriptValueImpl &v2) const
    {
        if (!v1.isValid() || v1.isUndefined())
            return false;
        if (!v2.isValid() || v2.isUndefined())
            return true;
        if (!m_comparefn.isUndefined()) {
            ArrayElementLessThan *that = const_cast<ArrayElementLessThan*>(this);
            QScriptValueImpl result = that->m_comparefn.call(QScriptValueImpl(),
                                                             QScriptValueImplList() << v1 << v2);
            return result.toNumber() <= 0;
        }
        return v1.toString() < v2.toString();
    }

private:
    QScriptValueImpl m_comparefn;
};

} // namespace QScript

inline QScript::Array::Array(QScriptEnginePrivate *engine):
    m_engine(engine),
    m_mode(VectorMode),
    m_instances(0)
{
    to_vector = new QVector<QScriptValueImpl>();
}

inline QScript::Array::Array(const Array &other):
    m_engine(other.m_engine),
    m_mode(other.m_mode),
    m_instances(other.m_instances)
{
    if (m_mode == VectorMode)
        to_vector = new QVector<QScriptValueImpl> (*other.to_vector);
    else
        to_map = new QMap<uint, QScriptValueImpl> (*other.to_map);
}

inline QScript::Array::~Array()
{
    if (m_mode == VectorMode)
        delete to_vector;
    else
        delete to_map;
}

inline QScript::Array &QScript::Array::operator = (const Array &other)
{
    m_engine = other.m_engine;
    m_instances = other.m_instances;
    if (m_mode != other.m_mode) {
        if (m_mode == VectorMode)
            delete to_vector;
        else
            delete to_map;
        m_mode = other.m_mode;

        if (m_mode == VectorMode)
            to_vector = new QVector<QScriptValueImpl> (*other.to_vector);
        else
            to_map = new QMap<uint, QScriptValueImpl> (*other.to_map);
    }

    if (m_mode == VectorMode)
        *to_vector = *other.to_vector;
    else
        *to_map = *other.to_map;

    return *this;
}

inline bool QScript::Array::isEmpty() const
{
    if (m_mode == VectorMode)
        return to_vector->isEmpty();

    return to_map->isEmpty();
}

inline uint QScript::Array::size() const
{
    if (m_mode == VectorMode)
        return to_vector->size();

    if (to_map->isEmpty())
        return 0;

    return (--to_map->constEnd()).key();
}

inline uint QScript::Array::count() const
{
    return size();
}

inline QScriptValueImpl QScript::Array::at(uint index) const
{
    if (m_mode == VectorMode) {
        if (index < uint(to_vector->size()))
            return to_vector->at(index);
        return QScriptValueImpl();
    } else {
        return to_map->value(index, QScriptValueImpl());
    }
}

inline void QScript::Array::assign(uint index, const QScriptValueImpl &v)
{
    if (index >= size()) {
        resize(index + 1);
        if (v.isValid() && m_engine)
            m_engine->adjustBytesAllocated(sizeof(QScriptValueImpl) * (size() - index));
    }

    const QScriptValueImpl &oldv = at(index);
    if (oldv.isValid() && (oldv.isObject() || oldv.isString()))
        --m_instances;

    if (v.isValid() && (v.isObject() || v.isString()))
        ++m_instances;

    if (m_mode == VectorMode) {
        to_vector->replace(index, v);
    } else {
        if (!v.isValid())
            to_map->remove(index);
        else
            to_map->insert(index, v);
    }
}

inline void QScript::Array::clear()
{
    m_instances = 0;

    if (m_mode == VectorMode)
        to_vector->clear();

    else
        to_map->clear();
}

inline void QScript::Array::mark(int generation)
{
    if (! m_instances)
        return;

    if (m_mode == VectorMode) {
        for (int i = 0; i < to_vector->size(); ++i)
            to_vector->at(i).mark(generation);
    } else {
        QMap<uint, QScriptValueImpl>::const_iterator it = to_map->constBegin();
        for (; it != to_map->constEnd(); ++it)
            it.value().mark(generation);
    }
}

inline void QScript::Array::resize(uint s)
{
    const uint oldSize = size();
    if (oldSize == s)
        return;

    const uint N = 10 * 1024;

    if (m_mode == VectorMode) {
        if (s < N) {
            to_vector->resize (s);
        } else {
            // switch to MapMode
            QMap<uint, QScriptValueImpl> *m = new QMap<uint, QScriptValueImpl>();
            for (uint i = 0; i < oldSize; ++i) {
                if (to_vector->at(i).isValid())
                    m->insert(i, to_vector->at(i));
            }
            m->insert(s, QScriptValueImpl());
            delete to_vector;
            to_map = m;
            m_mode = MapMode;
        }
    }

    else {
        if (s < N) {
            // switch to VectorMode
            QVector<QScriptValueImpl> *v = new QVector<QScriptValueImpl> (s);
            QMap<uint, QScriptValueImpl>::const_iterator it = to_map->constBegin();
            for ( ; (it != to_map->constEnd()) && (it.key() < s); ++it)
                (*v) [it.key()] = it.value();
            delete to_map;
            to_vector = v;
            m_mode = VectorMode;
        } else {
            if (!to_map->isEmpty()) {
                QMap<uint, QScriptValueImpl>::iterator it = --to_map->end();
                if (oldSize > s) {
                    // shrink
                    while ((it != to_map->end()) && (it.key() >= s)) {
                        it = to_map->erase(it);
                        --it;
                    }
                } else {
                    if ((it.key() == oldSize) && !it.value().isValid())
                        to_map->erase(it);
                }
            }
            to_map->insert(s, QScriptValueImpl());
        }
    }
}

inline void QScript::Array::concat(const QScript::Array &other)
{
    uint k = size();
    resize (k + other.size());
    for (uint i = 0; i < other.size(); ++i) {
        QScriptValueImpl v = other.at(i);
        if (! v.isValid())
            continue;

        assign(k + i, v);
    }
}

inline QScriptValueImpl QScript::Array::pop()
{
    if (isEmpty())
        return QScriptValueImpl();

    QScriptValueImpl v;

    if (m_mode == VectorMode)
        v = to_vector->last();
    else
        v = *--to_map->end();

    resize(size() - 1);

    return v;
}

inline void QScript::Array::sort(const QScriptValueImpl &comparefn)
{
    ArrayElementLessThan lessThan(comparefn);
    if (m_mode == VectorMode) {
        qSort(to_vector->begin(), to_vector->end(), lessThan);
    } else {
        QList<uint> keys = to_map->keys();
        QList<QScriptValueImpl> values = to_map->values();
        qStableSort(values.begin(), values.end(), lessThan);
        const uint len = keys.size();
        for (uint i = 0; i < len; ++i)
            to_map->insert(keys.at(i), values.at(i));
    }
}

inline void QScript::Array::splice(qsreal start, qsreal deleteCount,
                                   const QVector<QScriptValueImpl> &items,
                                   Array &other)
{
    const qsreal len = size();
    if (start < 0)
        start = qMax(len + start, qsreal(0));
    else if (start > len)
        start = len;
    deleteCount = qMax(qMin(deleteCount, len - start), qsreal(0));

    const uint st = uint(start);
    const uint dc = uint(deleteCount);
    other.resize(dc);

    const uint itemsSize = uint(items.size());

    if (m_mode == VectorMode) {
        for (uint i = 0; i < dc; ++i)
            other.assign(i, to_vector->at(st + i));
        if (itemsSize > dc)
            to_vector->insert(st, itemsSize - dc, QScriptValueImpl());
        else if (itemsSize < dc)
            to_vector->remove(st, dc - itemsSize);
        for (uint i = 0; i < itemsSize; ++i)
            to_vector->replace(st + i, items.at(i));
    } else {
        for (uint i = 0; i < dc; ++i)
            other.assign(i, to_map->take(st + i));
        uint del = itemsSize - dc;
        if (del != 0) {
            for (uint i = st; i < uint(len); ++i) {
                if (to_map->contains(i))
                    to_map->insert(i + del, to_map->take(i));
            }
            resize(uint(len) + del);
        }
        for (uint i = 0; i < itemsSize; ++i)
            to_map->insert(st + i, items.at(i));
    }
}

inline QList<uint> QScript::Array::keys() const
{
    if (m_mode == VectorMode)
        return QList<uint>();
    else
        return to_map->keys();
}

QT_END_NAMESPACE


#endif // QSCRIPTARRAY_P_H
