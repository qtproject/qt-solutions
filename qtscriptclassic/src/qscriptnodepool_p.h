// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#ifndef QSCRIPTNODEPOOL_P_H
#define QSCRIPTNODEPOOL_P_H

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

#include <QHash>
#include <QString>

#include "qscriptmemorypool_p.h"

QT_BEGIN_NAMESPACE

class QScriptEnginePrivate;

namespace QScript {

namespace AST {
class Node;
} // namespace AST

class Code;
class CompilationUnit;

template <typename NodeType>
inline NodeType *makeAstNode(MemoryPool *storage)
{
    NodeType *node = new (storage->allocate(sizeof(NodeType))) NodeType();
    return node;
}

template <typename NodeType, typename Arg1>
inline NodeType *makeAstNode(MemoryPool *storage, Arg1 arg1)
{
    NodeType *node = new (storage->allocate(sizeof(NodeType))) NodeType(arg1);
    return node;
}

template <typename NodeType, typename Arg1, typename Arg2>
inline NodeType *makeAstNode(MemoryPool *storage, Arg1 arg1, Arg2 arg2)
{
    NodeType *node = new (storage->allocate(sizeof(NodeType))) NodeType(arg1, arg2);
    return node;
}

template <typename NodeType, typename Arg1, typename Arg2, typename Arg3>
inline NodeType *makeAstNode(MemoryPool *storage, Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
    NodeType *node = new (storage->allocate(sizeof(NodeType))) NodeType(arg1, arg2, arg3);
    return node;
}

template <typename NodeType, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline NodeType *makeAstNode(MemoryPool *storage, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
    NodeType *node = new (storage->allocate(sizeof(NodeType))) NodeType(arg1, arg2, arg3, arg4);
    return node;
}

class NodePool : public MemoryPool
{
public:
    NodePool(const QString &fileName, QScriptEnginePrivate *engine);
    virtual ~NodePool();

    Code *createCompiledCode(AST::Node *node, CompilationUnit &compilation);

    inline QString fileName() const { return m_fileName; }
    inline QScriptEnginePrivate *engine() const { return m_engine; }
#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    inline qint64 id() const { return m_id; }
#endif

private:
    QHash<AST::Node*, Code*> m_codeCache;
    QString m_fileName;
    QScriptEnginePrivate *m_engine;
#ifndef Q_SCRIPT_NO_EVENT_NOTIFY
    qint64 m_id;
#endif

private:
    Q_DISABLE_COPY(NodePool)
};

} // namespace QScript

QT_END_NAMESPACE

#endif
