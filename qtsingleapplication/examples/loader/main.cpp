// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <qtsingleapplication.h>
#include <QFile>
#include <QMainWindow>
#include <QPrinter>
#include <QPainter>
#include <QTextEdit>
#include <QMdiArea>
#include <QTextStream>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    void handleMessage(const QString& message);

signals:
    void needToShow();

private:
    QMdiArea *workspace;
};

MainWindow::MainWindow()
{
    workspace = new QMdiArea(this);

    setCentralWidget(workspace);
}

void MainWindow::handleMessage(const QString& message)
{
    enum Action {
	Nothing,
	Open,
	Print
    } action;

    action = Nothing;
    QString filename = message;
    if (message.toLower().startsWith("/print ")) {
	filename = filename.mid(7);
	action = Print;
    } else if (!message.isEmpty()) {
	action = Open;
    }
    if (action == Nothing) {
        emit needToShow();
	return;
    }

    QFile file(filename);
    QString contents;
    if (file.open(QIODevice::ReadOnly))
        contents = file.readAll();
    else
        contents = "[[Error: Could not load file " + filename + "]]";

    QTextEdit *view = new QTextEdit;
    view->setPlainText(contents);

    switch(action) {
    case Print:
	{
	    QPrinter printer;
            view->print(&printer);
            delete view;
        }
	break;

    case Open:
	{
	    workspace->addSubWindow(view);
	    view->setWindowTitle(message);
	    view->show();
            emit needToShow();
	}
	break;
    default:
	break;
    };
}

#include "main.moc"

int main(int argc, char **argv)
{
    QtSingleApplication instance("File loader QtSingleApplication example", argc, argv);
    QString message;
    for (int a = 1; a < argc; ++a) {
	message += argv[a];
	if (a < argc-1)
	    message += " ";
    }

    if (instance.sendMessage(message))
	return 0;

    MainWindow mw;
    mw.handleMessage(message);
    mw.show();

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
		     &mw, SLOT(handleMessage(const QString&)));

    instance.setActivationWindow(&mw, false);
    QObject::connect(&mw, SIGNAL(needToShow()), &instance, SLOT(activateWindow()));

    return instance.exec();
}
