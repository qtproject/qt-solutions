// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include "qtunixserversocket.h"
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#ifndef SUN_LEN
#define SUN_LEN(ptr) ((size_t)(((struct sockaddr_un *) 0)->sun_path) \
                      +strlen ((ptr)->sun_path))
#endif

QtUnixServerSocket::QtUnixServerSocket(const QString &path, QObject *parent)
    : QTcpServer(parent)
{
    setPath(path);
}

QtUnixServerSocket::QtUnixServerSocket(QObject *parent)
    : QTcpServer(parent)
{
}

void QtUnixServerSocket::setPath(const QString &path)
{
    path_.clear();

    int sock = ::socket(PF_UNIX, SOCK_STREAM, 0);
    if (sock != -1) {
	struct sockaddr_un addr;
	::memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	::unlink(path.toLatin1().constData()); // ### This might need to be changed
	unsigned int pathlen = strlen(path.toLatin1().constData());
	if (pathlen > sizeof(addr.sun_path)) pathlen = sizeof(addr.sun_path);
	::memcpy(addr.sun_path, path.toLatin1().constData(), pathlen);
	if ((::bind(sock, (struct sockaddr *)&addr, SUN_LEN(&addr)) != -1) &&
	    (::listen(sock, 5) != -1)) {
	    setSocketDescriptor(sock);
            path_ = path;
        }
    }
}

void QtUnixServerSocket::close()
{
    QTcpServer::close();
    if (!path_.isEmpty()) {
        ::unlink(path_.toLatin1().constData());
        path_.clear();
    }
}
