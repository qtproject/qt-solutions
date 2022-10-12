// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <QApplication>
#include "mainframe.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainFrame frame;
    frame.show();

    return app.exec();
}
