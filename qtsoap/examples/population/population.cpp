// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <QApplication>
#include "population.h"

Population::Population(const QString &country, QObject *parent)
    : QObject(parent), http(this)
{
    // Connect the HTTP transport's responseReady() signal.
    connect(&http, SIGNAL(responseReady()), this, SLOT(getResponse()));

    // Construct a method request message.
    QtSoapMessage request;

    // Set the method and add one argument.
    request.setMethod("getPopulation", "http://www.abundanttech.com/WebServices/Population");
    request.addMethodArgument("strCountry", "", country);

    // Submit the request the the web service.
    http.setHost("www.abundanttech.com");
    http.setAction("http://www.abundanttech.com/WebServices/Population/getPopulation");
    http.submitRequest(request, "/WebServices/Population/population.asmx");

    qDebug("Looking up population of %s...", country.toLatin1().constData());
}

void Population::getResponse()
{
    // Get a reference to the response message.
    const QtSoapMessage &message = http.getResponse();

    // Check if the response is a SOAP Fault message
    if (message.isFault()) {
        qDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
    }
    else {
        // Get the return value, and print the result.
        const QtSoapType &response = message.returnValue();
        qDebug("%s has a population of %s (last updated at %s)",
               response["Country"].value().toString().toLatin1().constData(),
               response["Pop"].value().toString().toLatin1().constData(),
               response["Date"].value().toString().toLatin1().constData());
    }
    qApp->quit();
}
