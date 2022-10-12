// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <QTextStream>
#include <qtlockedfile.h>


int main()
{
    QTextStream qout(stdout);
    QTextStream qin(stdin);

    qout << "---===>>>> File locking example <<<<===---\n";

    QtLockedFile lf("foo");
    lf.open(QFile::ReadWrite);

    QString line;
    bool blocking = true;
    while (line != "q") {
        int m = lf.lockMode();

        if (m == 0)
            qout << "\n[*] You have no locks.";
        else if (m == QFile::ReadOnly)
            qout << "\n[*] You have a read lock.";
        else
            qout << "\n[*] You have a read/write lock.";
        qout << " Blocking wait is ";
        if (blocking)
            qout << "ON.\n";
        else
            qout << "OFF.\n";

        qout << "Acquire [r]ead lock, read/[w]rite lock, re[l]ease lock, [t]oggle or [q]uit? ";
        qout.flush();

        line = qin.readLine();
        if (line.isNull())
            break;

        if (line == "r") {
            qout << "Acquiring a read lock... ";
            qout.flush();
            if (lf.lock(QtLockedFile::ReadLock, blocking))
                qout << "done!\n";
            else
                qout << "not currently possible!\n";
        } else if (line == "w") {
            qout << "Acquiring a read/write lock... ";
            qout.flush();
            if (lf.lock(QtLockedFile::WriteLock, blocking))
                qout << "done!\n";
            else
                qout << "not currently possible!\n";
        } else if (line == "l") {
            qout << "Releasing lock... ";
            qout.flush();
            lf.unlock();
            qout << "done!\n";
        } else if (line == "t") {
            blocking = !blocking;
        }

        qout.flush();
    }
}
