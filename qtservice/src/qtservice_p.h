// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#ifndef QTSERVICE_P_H
#define QTSERVICE_P_H

#include "qtservice.h"

#include <QStringList>

#include <vector>

class QtServiceControllerPrivate
{
    Q_DECLARE_PUBLIC(QtServiceController)
public:
    QString serviceName;
    QtServiceController *q_ptr;
};

class QtServiceBasePrivate
{
    Q_DECLARE_PUBLIC(QtServiceBase)
public:

    QtServiceBasePrivate(const QString &name);
    ~QtServiceBasePrivate();

    QtServiceBase *q_ptr;

    QString displayName;
    QString serviceDescription;
    QtServiceController::StartupType startupType;
    QtServiceBase::ServiceFlags serviceFlags;
    QStringList args;

    std::vector<QtServiceBase::PreHook>  PreInstallHooks;
    std::vector<QtServiceBase::PostHook> PostInstallHooks;
    std::vector<QtServiceBase::PreHook > PreUninstallHooks;
    std::vector<QtServiceBase::PostHook> PostUninstallHooks;

    static class QtServiceBase *instance;

    QtServiceController controller;

    void startService();
    int run(bool asService, const QStringList &argList);
    bool install(const QString &account, const QString &password);

    bool start();

    QString filePath() const;
    bool sysInit();
    void sysSetPath();
    void sysCleanup();
    class QtServiceSysPrivate *sysd;
};

#endif
