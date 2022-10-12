// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

/*!
    This example shows QtSoap with a .NET web service.

    Gets the US Census Bureau population from any of 227 countries,
    the daily world population estimate, and the list of available
    countries.
*/
#include <QApplication>
#include "population.h"

#define USAGE1 QT_TRANSLATE_NOOP("Population::main", "XMethods' interface to a World Population .NET service.")
#define USAGE2 QT_TRANSLATE_NOOP("Population::main", "usage: %s <country>")


int main(int argc, char **argv)
{
    if (argc < 2) {
	qDebug(USAGE1);
	qDebug(USAGE2, argv[0]);
	qDebug(" ");

	return 1;
    }

    QApplication app(argc, argv, false);

    Population pop(argv[1], 0);

    return app.exec();
}
