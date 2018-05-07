/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: https://www1.qt.io/terms-conditions
**
** This file is part of the Qt Solutions component.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QSCRIPTPRETTYPRETTY_P_H
#define QSCRIPTPRETTYPRETTY_P_H

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

#include <qglobal.h>

#include "qscriptastvisitor_p.h"

QT_BEGIN_NAMESPACE

class QTextStream;

namespace QScript {

class PrettyPretty: protected AST::Visitor
{
public:
    PrettyPretty(QTextStream &out);
    virtual ~PrettyPretty();

    QTextStream &operator () (AST::Node *node, int level = 0);

protected:
    void accept(AST::Node *node);

    virtual bool preVisit(AST::Node *node);

    virtual bool visit(AST::ThisExpression *node);
    virtual void endVisit(AST::ThisExpression *node);

    virtual bool visit(AST::IdentifierExpression *node);
    virtual void endVisit(AST::IdentifierExpression *node);

    virtual bool visit(AST::NullExpression *node);
    virtual void endVisit(AST::NullExpression *node);

    virtual bool visit(AST::TrueLiteral *node);
    virtual void endVisit(AST::TrueLiteral *node);

    virtual bool visit(AST::FalseLiteral *node);
    virtual void endVisit(AST::FalseLiteral *node);

    virtual bool visit(AST::StringLiteral *node);
    virtual void endVisit(AST::StringLiteral *node);

    virtual bool visit(AST::NumericLiteral *node);
    virtual void endVisit(AST::NumericLiteral *node);

    virtual bool visit(AST::RegExpLiteral *node);
    virtual void endVisit(AST::RegExpLiteral *node);

    virtual bool visit(AST::ArrayLiteral *node);
    virtual void endVisit(AST::ArrayLiteral *node);

    virtual bool visit(AST::ObjectLiteral *node);
    virtual void endVisit(AST::ObjectLiteral *node);

    virtual bool visit(AST::ElementList *node);
    virtual void endVisit(AST::ElementList *node);

    virtual bool visit(AST::Elision *node);
    virtual void endVisit(AST::Elision *node);

    virtual bool visit(AST::PropertyNameAndValueList *node);
    virtual void endVisit(AST::PropertyNameAndValueList *node);

    virtual bool visit(AST::IdentifierPropertyName *node);
    virtual void endVisit(AST::IdentifierPropertyName *node);

    virtual bool visit(AST::StringLiteralPropertyName *node);
    virtual void endVisit(AST::StringLiteralPropertyName *node);

    virtual bool visit(AST::NumericLiteralPropertyName *node);
    virtual void endVisit(AST::NumericLiteralPropertyName *node);

    virtual bool visit(AST::ArrayMemberExpression *node);
    virtual void endVisit(AST::ArrayMemberExpression *node);

    virtual bool visit(AST::FieldMemberExpression *node);
    virtual void endVisit(AST::FieldMemberExpression *node);

    virtual bool visit(AST::NewMemberExpression *node);
    virtual void endVisit(AST::NewMemberExpression *node);

    virtual bool visit(AST::NewExpression *node);
    virtual void endVisit(AST::NewExpression *node);

    virtual bool visit(AST::CallExpression *node);
    virtual void endVisit(AST::CallExpression *node);

    virtual bool visit(AST::ArgumentList *node);
    virtual void endVisit(AST::ArgumentList *node);

    virtual bool visit(AST::PostIncrementExpression *node);
    virtual void endVisit(AST::PostIncrementExpression *node);

    virtual bool visit(AST::PostDecrementExpression *node);
    virtual void endVisit(AST::PostDecrementExpression *node);

    virtual bool visit(AST::DeleteExpression *node);
    virtual void endVisit(AST::DeleteExpression *node);

    virtual bool visit(AST::VoidExpression *node);
    virtual void endVisit(AST::VoidExpression *node);

    virtual bool visit(AST::TypeOfExpression *node);
    virtual void endVisit(AST::TypeOfExpression *node);

    virtual bool visit(AST::PreIncrementExpression *node);
    virtual void endVisit(AST::PreIncrementExpression *node);

    virtual bool visit(AST::PreDecrementExpression *node);
    virtual void endVisit(AST::PreDecrementExpression *node);

    virtual bool visit(AST::UnaryPlusExpression *node);
    virtual void endVisit(AST::UnaryPlusExpression *node);

    virtual bool visit(AST::UnaryMinusExpression *node);
    virtual void endVisit(AST::UnaryMinusExpression *node);

    virtual bool visit(AST::TildeExpression *node);
    virtual void endVisit(AST::TildeExpression *node);

    virtual bool visit(AST::NotExpression *node);
    virtual void endVisit(AST::NotExpression *node);

    virtual bool visit(AST::BinaryExpression *node);
    virtual void endVisit(AST::BinaryExpression *node);

    virtual bool visit(AST::ConditionalExpression *node);
    virtual void endVisit(AST::ConditionalExpression *node);

    virtual bool visit(AST::Expression *node);
    virtual void endVisit(AST::Expression *node);

    virtual bool visit(AST::Block *node);
    virtual void endVisit(AST::Block *node);

    virtual bool visit(AST::StatementList *node);
    virtual void endVisit(AST::StatementList *node);

    virtual bool visit(AST::VariableStatement *node);
    virtual void endVisit(AST::VariableStatement *node);

    virtual bool visit(AST::VariableDeclarationList *node);
    virtual void endVisit(AST::VariableDeclarationList *node);

    virtual bool visit(AST::VariableDeclaration *node);
    virtual void endVisit(AST::VariableDeclaration *node);

    virtual bool visit(AST::EmptyStatement *node);
    virtual void endVisit(AST::EmptyStatement *node);

    virtual bool visit(AST::ExpressionStatement *node);
    virtual void endVisit(AST::ExpressionStatement *node);

    virtual bool visit(AST::IfStatement *node);
    virtual void endVisit(AST::IfStatement *node);

    virtual bool visit(AST::DoWhileStatement *node);
    virtual void endVisit(AST::DoWhileStatement *node);

    virtual bool visit(AST::WhileStatement *node);
    virtual void endVisit(AST::WhileStatement *node);

    virtual bool visit(AST::ForStatement *node);
    virtual void endVisit(AST::ForStatement *node);

    virtual bool visit(AST::LocalForStatement *node);
    virtual void endVisit(AST::LocalForStatement *node);

    virtual bool visit(AST::ForEachStatement *node);
    virtual void endVisit(AST::ForEachStatement *node);

    virtual bool visit(AST::LocalForEachStatement *node);
    virtual void endVisit(AST::LocalForEachStatement *node);

    virtual bool visit(AST::ContinueStatement *node);
    virtual void endVisit(AST::ContinueStatement *node);

    virtual bool visit(AST::BreakStatement *node);
    virtual void endVisit(AST::BreakStatement *node);

    virtual bool visit(AST::ReturnStatement *node);
    virtual void endVisit(AST::ReturnStatement *node);

    virtual bool visit(AST::WithStatement *node);
    virtual void endVisit(AST::WithStatement *node);

    virtual bool visit(AST::SwitchStatement *node);
    virtual void endVisit(AST::SwitchStatement *node);

    virtual bool visit(AST::CaseBlock *node);
    virtual void endVisit(AST::CaseBlock *node);

    virtual bool visit(AST::CaseClauses *node);
    virtual void endVisit(AST::CaseClauses *node);

    virtual bool visit(AST::CaseClause *node);
    virtual void endVisit(AST::CaseClause *node);

    virtual bool visit(AST::DefaultClause *node);
    virtual void endVisit(AST::DefaultClause *node);

    virtual bool visit(AST::LabelledStatement *node);
    virtual void endVisit(AST::LabelledStatement *node);

    virtual bool visit(AST::ThrowStatement *node);
    virtual void endVisit(AST::ThrowStatement *node);

    virtual bool visit(AST::TryStatement *node);
    virtual void endVisit(AST::TryStatement *node);

    virtual bool visit(AST::Catch *node);
    virtual void endVisit(AST::Catch *node);

    virtual bool visit(AST::Finally *node);
    virtual void endVisit(AST::Finally *node);

    virtual bool visit(AST::FunctionDeclaration *node);
    virtual void endVisit(AST::FunctionDeclaration *node);

    virtual bool visit(AST::FunctionExpression *node);
    virtual void endVisit(AST::FunctionExpression *node);

    virtual bool visit(AST::FormalParameterList *node);
    virtual void endVisit(AST::FormalParameterList *node);

    virtual bool visit(AST::FunctionBody *node);
    virtual void endVisit(AST::FunctionBody *node);

    virtual bool visit(AST::Program *node);
    virtual void endVisit(AST::Program *node);

    virtual bool visit(AST::SourceElements *node);
    virtual void endVisit(AST::SourceElements *node);

    virtual bool visit(AST::FunctionSourceElement *node);
    virtual void endVisit(AST::FunctionSourceElement *node);

    virtual bool visit(AST::StatementSourceElement *node);
    virtual void endVisit(AST::StatementSourceElement *node);

    virtual bool visit(AST::DebuggerStatement *node);
    virtual void endVisit(AST::DebuggerStatement *node);

    int indentLevel(int level)
    {
        int was = m_indentLevel;
        m_indentLevel = level;
        return was;
    }

    void pushIndentLevel()
    { ++m_indentLevel; }

    void popIndentLevel()
    { --m_indentLevel; }

    QTextStream &newlineAndIndent();

    void acceptAsBlock(AST::Node *node);

    static int operatorPrecedenceLevel(int op);
    static int compareOperatorPrecedence(int op1, int op2);

private:
    QTextStream &out;
    int m_indentLevel;

    Q_DISABLE_COPY(PrettyPretty)
};

} // namespace QScript

QT_END_NAMESPACE

#endif
