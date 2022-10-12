// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <QCoreApplication>
#include <QLocale>
#include "easter.h"

Easter::Easter(short year, QObject *parent)
    : QObject(parent), http(this)
{
    connect(&http, SIGNAL(responseReady(const QtSoapMessage &)),
            this, SLOT(getResponse(const QtSoapMessage &)));

    QtSoapMessage request;
    request.setMethod("GetEaster",
		      "http://www.27seconds.com/Holidays/US/Dates/");
    request.addMethodArgument("year", "", year);

    http.setHost("www.27seconds.com");
    http.setAction("http://www.27seconds.com/Holidays/US/Dates/GetEaster");
    http.submitRequest(request, "/Holidays/US/Dates/USHolidayDates.asmx");

    qDebug("Looking up the date of easter in %i...", year);
    this->year = year;
}

void Easter::getResponse(const QtSoapMessage &message)
{
    if (message.isFault()) {
	qDebug("Error: %s", qPrintable(message.faultString().toString()));
    }
    else {
        QString res = message.returnValue().toString();
        QDateTime dt = QDateTime::fromString(res, Qt::ISODate);
        if (dt.isValid())
            res = QLocale::c().toString(dt.date());

        qDebug("Easter is: %s", res.toLatin1().constData());
    }
    QCoreApplication::quit();
}
