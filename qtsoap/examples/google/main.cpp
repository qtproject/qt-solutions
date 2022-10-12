// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <QApplication>
#include "google.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Google google;

    google.show();

    return app.exec();
}
