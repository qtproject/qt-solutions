// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include "qtsinglecoreapplication.h"
#include <QDebug>


void report(const QString& msg)
{
    qDebug("[%i] %s", (int)QCoreApplication::applicationPid(), qPrintable(msg));
}

class MainClass : public QObject
{
    Q_OBJECT
public:
    MainClass()
        : QObject()
        {}

public slots:
    void handleMessage(const QString& message)
        {
            report( "Message received: \"" + message + "\"");
        }
};

int main(int argc, char **argv)
{
    report("Starting up");

    QtSingleCoreApplication app(argc, argv);

    if (app.isRunning()) {
        QString msg(QString("Hi master, I am %1.").arg(QCoreApplication::applicationPid()));
        bool sentok = app.sendMessage(msg, 2000);
        QString rep("Another instance is running, so I will exit.");
        rep += sentok ? " Message sent ok." : " Message sending failed; the other instance may be frozen.";
        report(rep);
        return 0;
    } else {
        report("No other instance is running; so I will.");
        MainClass mainObj;
        QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                         &mainObj, SLOT(handleMessage(const QString&)));
        return app.exec();
    }
}


#include "main.moc"
