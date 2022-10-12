// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QGUARD_P_H
#define QGUARD_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp and qfiledialog.cpp.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qglobal.h"

QT_BEGIN_NAMESPACE

class QObject;
template<class T>
class QGuard
{
    QObject *o;
    QGuard<QObject> *next;
    QGuard<QObject> **prev;
    friend void q_guard_addGuard(QGuard<QObject> *);
    friend void q_guard_removeGuard(QGuard<QObject> *);
    friend class QObjectPrivate;
public:
    inline QGuard();
    inline QGuard(T *);
    inline QGuard(const QGuard<T> &);
    inline virtual ~QGuard();

    inline QGuard<T> &operator=(const QGuard<T> &o);
    inline QGuard<T> &operator=(T *);
    
    inline bool isNull() const
        { return !o; }

    inline T* operator->() const
        { return static_cast<T*>(const_cast<QObject*>(o)); }
    inline T& operator*() const
        { return *static_cast<T*>(const_cast<QObject*>(o)); }
    inline operator T*() const
        { return static_cast<T*>(const_cast<QObject*>(o)); }
    inline T* data() const
        { return static_cast<T*>(const_cast<QObject*>(o)); }

protected:
    virtual void objectDestroyed(T *) {}
};

QT_END_NAMESPACE

#include "private/qobject_p.h"

QT_BEGIN_NAMESPACE

inline void q_guard_addGuard(QGuard<QObject> *);
inline void q_guard_removeGuard(QGuard<QObject> *);

template<class T>
QGuard<T>::QGuard()
: o(0), next(0), prev(0)
{
}

template<class T>
QGuard<T>::QGuard(T *g)
: o(g), next(0), prev(0)
{
    if (o) q_guard_addGuard(reinterpret_cast<QGuard<QObject> *>(this));
}

template<class T>
QGuard<T>::QGuard(const QGuard<T> &g)
: o(g.o), next(0), prev(0)
{
    if (o) q_guard_addGuard(reinterpret_cast<QGuard<QObject> *>(this));
}

template<class T>
QGuard<T>::~QGuard()
{
    if (prev) q_guard_removeGuard(reinterpret_cast<QGuard<QObject> *>(this));
    o = 0;
}

template<class T>
QGuard<T> &QGuard<T>::operator=(const QGuard<T> &g)
{
    if (g.o != o) {
        if (prev) 
            q_guard_removeGuard(reinterpret_cast<QGuard<QObject> *>(this));
        o = g.o;
        if (o) q_guard_addGuard(reinterpret_cast<QGuard<QObject> *>(this));
    }
    return *this;
}

template<class T>
inline QGuard<T> &QGuard<T>::operator=(T *g)
{
    if (g != o) {
        if (prev) 
            q_guard_removeGuard(reinterpret_cast<QGuard<QObject> *>(this));
        o = g;
        if (o) q_guard_addGuard(reinterpret_cast<QGuard<QObject> *>(this));
    }
    return *this;
}

QT_END_NAMESPACE

#endif // QGUARD_P_H
