// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QOBJECT_P_H
#define QOBJECT_P_H

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

#include "QtCore/qobject.h"
#include "QtCore/qpointer.h"
#include "QtCore/qcoreevent.h"
#include "QtCore/qlist.h"
#include "QtCore/qvector.h"
#include "QtCore/qreadwritelock.h"
#include "QtCore/qvariant.h"
#include "private/qguard_p.h"

QT_BEGIN_NAMESPACE

class QVariant;
class QThreadData;
class QObjectConnectionListVector;
namespace QtSharedPointer { class ExternalRefCountData; }

/* mirrored in QtTestLib, DON'T CHANGE without prior warning */
struct QSignalSpyCallbackSet
{
    typedef void (*BeginCallback)(QObject *caller, int method_index, void **argv);
    typedef void (*EndCallback)(QObject *caller, int method_index);
    BeginCallback signal_begin_callback,
                    slot_begin_callback;
    EndCallback signal_end_callback,
                slot_end_callback;
};
void Q_CORE_EXPORT qt_register_signal_spy_callbacks(const QSignalSpyCallbackSet &callback_set);

extern QSignalSpyCallbackSet Q_CORE_EXPORT qt_signal_spy_callback_set;

enum { QObjectPrivateVersion = QT_VERSION };

class Q_CORE_EXPORT QDeclarativeData
{
public:
    virtual ~QDeclarativeData();
    virtual void destroyed(QObject *) = 0;
};

class Q_CORE_EXPORT QObjectPrivate : public QObjectData
{
    Q_DECLARE_PUBLIC(QObject)

public:
    struct ExtraData
    {
#ifndef QT_NO_USERDATA
        QVector<QObjectUserData *> userData;
#endif
        QList<QByteArray> propertyNames;
        QList<QVariant> propertyValues;
    };

    struct Connection
    {
        QObject *sender;
        QObject *receiver;
        int method;
        uint connectionType : 3; // 0 == auto, 1 == direct, 2 == queued, 4 == blocking
        QBasicAtomicPointer<int> argumentTypes;
        //senders linked list
        Connection *next;
        Connection **prev;
        ~Connection();
    };
    typedef QList<Connection *> ConnectionList;

    struct Sender
    {
        QObject *sender;
        int signal;
        int ref;
    };


    QObjectPrivate(int version = QObjectPrivateVersion);
    virtual ~QObjectPrivate();
    void deleteChildren();

    void setParent_helper(QObject *);
    void moveToThread_helper();
    void setThreadData_helper(QThreadData *currentData, QThreadData *targetData);
    void _q_reregisterTimers(void *pointer);

    bool isSender(const QObject *receiver, const char *signal) const;
    QObjectList receiverList(const char *signal) const;
    QObjectList senderList() const;

    void addConnection(int signal, Connection *c);
    void cleanConnectionLists();

#ifdef QT3_SUPPORT
    void sendPendingChildInsertedEvents();
    void removePendingChildInsertedEvents(QObject *child);
#endif

    static Sender *setCurrentSender(QObject *receiver,
                                    Sender *sender);
    static void resetCurrentSender(QObject *receiver,
                                   Sender *currentSender,
                                   Sender *previousSender);
    static int *setDeleteWatch(QObjectPrivate *d, int *newWatch);
    static void resetDeleteWatch(QObjectPrivate *d, int *oldWatch, int deleteWatch);
    static void clearGuards(QObject *);

    static QObjectPrivate *get(QObject *o) {
        return o->d_func();
    }

public:
    QString objectName;
    ExtraData *extraData;    // extra data set by the user
    QThreadData *threadData; // id of the thread that owns the object

    QObjectConnectionListVector *connectionLists;

    Connection *senders;     // linked list of connections connected to this object
    Sender *currentSender;   // object currently activating the object
    mutable quint32 connectedSignals[2];   // 64-bit, so doesn't cause padding on 64-bit platforms

#ifdef QT3_SUPPORT
    QList<QObject *> pendingChildInsertedEvents;
#else
    // preserve binary compatibility with code compiled without Qt 3 support
    // ### why?
    QList<QObject *> unused;
#endif

    QList<QPointer<QObject> > eventFilters;
    QObject *currentChildBeingDeleted;

    // these objects are all used to indicate that a QObject was deleted
    // plus QPointer, which keeps a separate list
    QDeclarativeData *declarativeData;
    QGuard<QObject> *objectGuards;
    QAtomicPointer<QtSharedPointer::ExternalRefCountData> sharedRefcount;
    int *deleteWatch;
};

inline void q_guard_addGuard(QGuard<QObject> *g)
{
    QObjectPrivate *p = QObjectPrivate::get(g->o);
    if (p->wasDeleted) {
        qWarning("QGuard: cannot add guard to deleted object");
        g->o = 0;
        return;
    }

    g->next = p->objectGuards;
    p->objectGuards = g;
    g->prev = &p->objectGuards;
    if (g->next)
        g->next->prev = &g->next;
}

inline void q_guard_removeGuard(QGuard<QObject> *g)
{
    if (g->next) g->next->prev = g->prev;
    *g->prev = g->next;
    g->next = 0;
    g->prev = 0;
}

Q_DECLARE_TYPEINFO(QObjectPrivate::Connection, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QObjectPrivate::Sender, Q_MOVABLE_TYPE);

class QSemaphore;
class Q_CORE_EXPORT QMetaCallEvent : public QEvent
{
public:
    QMetaCallEvent(int id, const QObject *sender, int signalId,
                   int nargs = 0, int *types = 0, void **args = 0, QSemaphore *semaphore = 0);
    ~QMetaCallEvent();

    inline int id() const { return id_; }
    inline const QObject *sender() const { return sender_; }
    inline int signalId() const { return signalId_; }
    inline void **args() const { return args_; }

    virtual int placeMetaCall(QObject *object);

private:
    int id_;
    const QObject *sender_;
    int signalId_;
    int nargs_;
    int *types_;
    void **args_;
    QSemaphore *semaphore_;
};

class QBoolBlocker
{
public:
    inline QBoolBlocker(bool &b, bool value=true):block(b), reset(b){block = value;}
    inline ~QBoolBlocker(){block = reset; }
private:
    bool &block;
    bool reset;
};

void Q_CORE_EXPORT qDeleteInEventHandler(QObject *o);


struct Q_CORE_EXPORT QAbstractDynamicMetaObject : public QMetaObject
{
    virtual ~QAbstractDynamicMetaObject() {}
    virtual int metaCall(QMetaObject::Call, int _id, void **) { return _id; }
    virtual int createProperty(const char *, const char *) { return -1; }
};

QT_END_NAMESPACE

#endif // QOBJECT_P_H
