// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <qtsingleapplication.h>
#include <QTextEdit>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = 0)
        : QTextEdit(parent)
    {}
public slots:
    void append(const QString &str)
    {
        QTextEdit::append(str);
    }
};

#include "main.moc"



int main(int argc, char **argv)
{
    QtSingleApplication instance(argc, argv);
    if (instance.sendMessage("Wake up!"))
	return 0;

    TextEdit logview;
    logview.setReadOnly(true);
    logview.show();

    instance.setActivationWindow(&logview);

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
		     &logview, SLOT(append(const QString&)));

    return instance.exec();
}
