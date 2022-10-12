// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTREPOSITORY_P_H
#define QSCRIPTREPOSITORY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qscriptbuffer_p.h"

QT_BEGIN_NAMESPACE

namespace QScript {

template <typename Tp, typename Factory>
class Repository
{
public:
    inline Repository() { cache.reserve(32); }
    inline ~Repository() { qDeleteAll(cache); }

    inline Tp *get()
    {
        if (cache.isEmpty())
            return Factory::create();

        return cache.takeLast();
    }

    inline void release(Tp *item)
    { cache.append(item); }

private:
    Buffer<Tp*> cache;

private:
    Q_DISABLE_COPY(Repository)
};

} // namespace QScript

QT_END_NAMESPACE

#endif
