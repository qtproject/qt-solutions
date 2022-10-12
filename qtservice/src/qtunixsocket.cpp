// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause

#include "qtunixsocket.h"
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#ifndef SUN_LEN
#define SUN_LEN(ptr) ((size_t)(((struct sockaddr_un *) 0)->sun_path) \
                      +strlen ((ptr)->sun_path))
#endif

QtUnixSocket::QtUnixSocket(QObject *parent)
    : QTcpSocket(parent)
{
}

bool QtUnixSocket::connectTo(const QString &path)
{
    bool ret = false;
    int sock = ::socket(PF_UNIX, SOCK_STREAM, 0);
    if (sock != -1) {
        struct sockaddr_un addr;
	::memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	size_t pathlen = strlen(path.toLatin1().constData());
        pathlen = qMin(pathlen, sizeof(addr.sun_path));
	::memcpy(addr.sun_path, path.toLatin1().constData(), pathlen);
	int err = ::connect(sock, (struct sockaddr *)&addr, SUN_LEN(&addr));
        if (err != -1) {
            setSocketDescriptor(sock);
	    ret = true;
	} else {
            ::close(sock);
        }
    }
    return ret;
}
