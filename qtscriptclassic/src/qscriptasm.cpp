// Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
// SPDX-License-Identifier: BSD-3-Clause


#include <QTextStream>


#include "qscriptasm_p.h"
#include "qscriptvalueimpl_p.h"
#include "qscriptengine_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"

QT_BEGIN_NAMESPACE

const char *QScriptInstruction::opcode[] = {
#define STR(a) #a
#define Q_SCRIPT_DEFINE_OPERATOR(op) STR(i##op) ,
#include "instruction.table"
#undef Q_SCRIPT_DEFINE_OPERATOR
#undef STR
};

void QScriptInstruction::print(QTextStream &out) const
{
    out << opcode[op];

    if (! operand[0].isValid())
        return;

    out << '(' << operand[0].toString();

    if (operand[1].isValid())
        out << ", " << operand[1].toString();

    out << ')';
}

namespace QScript {

Code::Code():
    optimized(false),
    firstInstruction(0),
    lastInstruction(0),
    astPool(0)
{
}

Code::~Code()
{
    delete[] firstInstruction;
}

void Code::init(const CompilationUnit &compilation, NodePool *pool)
{
    optimized = false;
    const QVector<QScriptInstruction> ilist = compilation.instructions();
    firstInstruction = new QScriptInstruction[ilist.count()];
    lastInstruction = firstInstruction + ilist.count();
    qCopy(ilist.begin(), ilist.end(), firstInstruction);
    exceptionHandlers = compilation.exceptionHandlers();
    astPool = pool;
}

} // namespace QScript

QT_END_NAMESPACE

