// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QtScript>

//! [0]
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(helloscript);
//! [0]

//! [1]
    QApplication app(argc, argv);

    QScriptEngine engine;

    QTranslator translator;
    translator.load("helloscript_la");
    app.installTranslator(&translator);
    engine.installTranslatorFunctions();
//! [1]

//! [2]
    QPushButton button;
    QScriptValue scriptButton = engine.newQObject(&button);
    engine.globalObject().setProperty("button", scriptButton);
//! [2]

//! [3]
    QString fileName(":/helloscript.qs");
    QFile scriptFile(fileName);
    scriptFile.open(QIODevice::ReadOnly);
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();
//! [3]

//! [4]
    QScriptValue result = engine.evaluate(contents, fileName);
//! [4]

//! [5]
    if (result.isError()) {
        QMessageBox::critical(0, "Hello Script",
                              QString::fromLatin1("%0:%1: %2")
                              .arg(fileName)
                              .arg(result.property("lineNumber").toInt32())
                              .arg(result.toString()));
        return -1;
    }
//! [5]

//! [6]
    return app.exec();
}
//! [6]
