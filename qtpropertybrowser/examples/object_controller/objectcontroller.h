// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef OBJECTCONTROLLER_H
#define OBJECTCONTROLLER_H

#include <QWidget>

class ObjectControllerPrivate;

class ObjectController : public QWidget
{
    Q_OBJECT
public:
    ObjectController(QWidget *parent = 0);
    ~ObjectController();

    void setObject(QObject *object);
    QObject *object() const;

private:
    ObjectControllerPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ObjectController)
    Q_DISABLE_COPY(ObjectController)
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QVariant &))
};

#endif
