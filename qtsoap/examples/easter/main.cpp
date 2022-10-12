// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

/*!
    This example shows QtSoap with a SOAP::Lite web service.

    Returns the date of easter in a given year.
*/
#include <QCoreApplication>
#include <QFile>
#include <stdlib.h>
#include "easter.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
	qDebug("XMethods' interface to Easter Dates with a SOAP::Lite service.");
	qDebug("usage: %s <year>", argv[0]);
	qDebug(" ");

	return 1;
    }

    QCoreApplication app(argc, argv);

    Easter easter(atoi(argv[1]), 0);

    return app.exec();
}

