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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "qscriptengine_p.h"


#include "qscriptvalueimpl_p.h"
#include "qscriptcontext_p.h"
#include "qscriptmember_p.h"
#include "qscriptobject_p.h"
#include "qscriptlexer_p.h"
#include "qscriptgrammar_p.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

QT_BEGIN_NAMESPACE

extern double qstrtod(const char *s00, char const **se, bool *ok);

#define shiftWindowsLineBreak() \
    do { \
        if (((current == '\r') && (next1 == '\n')) \
            || ((current == '\n') && (next1 == '\r'))) { \
            shift(1); \
        } \
    } \
    while (0)

namespace QScript {
extern qsreal integerFromString(const char *buf, int size, int radix);
}

QScript::Lexer::Lexer(QScriptEnginePrivate *eng)
    : driver(eng),
      yylineno(0),
      done(false),
      size8(128), size16(128),
      pos8(0), pos16(0),
      terminator(false),
      restrKeyword(false),
      delimited(false),
      stackToken(-1),
      state(Start),
      pos(0),
      code(0), length(0),
      yycolumn(0),
      startlineno(0), startcolumn(0),
      bol(true),
      current(0), next1(0), next2(0), next3(0),
      err(NoError),
      wantRx(false),
      check_reserved(true),
      parenthesesState(IgnoreParentheses),
      parenthesesCount(0),
      prohibitAutomaticSemicolon(false)
{
    // allocate space for read buffers
    buffer8 = new char[size8];
    buffer16 = new QChar[size16];
    pattern = 0;
    flags = 0;

}

QScript::Lexer::~Lexer()
{
    delete [] buffer8;
    delete [] buffer16;
}

void QScript::Lexer::setCode(const QString &c, int lineno)
{
    errmsg = QString();
    yylineno = lineno;
    yycolumn = 1;
    restrKeyword = false;
    delimited = false;
    stackToken = -1;
    pos = 0;
    code = c.unicode();
    length = c.length();
    bol = true;

    // read first characters
    current = (length > 0) ? code[0].unicode() : 0;
    next1 = (length > 1) ? code[1].unicode() : 0;
    next2 = (length > 2) ? code[2].unicode() : 0;
    next3 = (length > 3) ? code[3].unicode() : 0;
}

void QScript::Lexer::shift(uint p)
{
    while (p--) {
        ++pos;
        ++yycolumn;
        current = next1;
        next1 = next2;
        next2 = next3;
        next3 = (pos + 3 < length) ? code[pos+3].unicode() : 0;
    }
}

void QScript::Lexer::setDone(State s)
{
    state = s;
    done = true;
}

int QScript::Lexer::findReservedWord(const QChar *c, int size) const
{
    switch (size) {
    case 2: {
        if (c[0] == QLatin1Char('d') && c[1] == QLatin1Char('o'))
            return QScriptGrammar::T_DO;
        else if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('f'))
            return QScriptGrammar::T_IF;
        else if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('n'))
            return QScriptGrammar::T_IN;
    }   break;

    case 3: {
        if (c[0] == QLatin1Char('f') && c[1] == QLatin1Char('o') && c[2] == QLatin1Char('r'))
            return QScriptGrammar::T_FOR;
        else if (c[0] == QLatin1Char('n') && c[1] == QLatin1Char('e') && c[2] == QLatin1Char('w'))
            return QScriptGrammar::T_NEW;
        else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('r') && c[2] == QLatin1Char('y'))
            return QScriptGrammar::T_TRY;
        else if (c[0] == QLatin1Char('v') && c[1] == QLatin1Char('a') && c[2] == QLatin1Char('r'))
            return QScriptGrammar::T_VAR;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('n') && c[2] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 4: {
        if (c[0] == QLatin1Char('c') && c[1] == QLatin1Char('a')
                && c[2] == QLatin1Char('s') && c[3] == QLatin1Char('e'))
            return QScriptGrammar::T_CASE;
        else if (c[0] == QLatin1Char('e') && c[1] == QLatin1Char('l')
                && c[2] == QLatin1Char('s') && c[3] == QLatin1Char('e'))
            return QScriptGrammar::T_ELSE;
        else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('h')
                && c[2] == QLatin1Char('i') && c[3] == QLatin1Char('s'))
            return QScriptGrammar::T_THIS;
        else if (c[0] == QLatin1Char('v') && c[1] == QLatin1Char('o')
                && c[2] == QLatin1Char('i') && c[3] == QLatin1Char('d'))
            return QScriptGrammar::T_VOID;
        else if (c[0] == QLatin1Char('w') && c[1] == QLatin1Char('i')
                && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('h'))
            return QScriptGrammar::T_WITH;
        else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('r')
                && c[2] == QLatin1Char('u') && c[3] == QLatin1Char('e'))
            return QScriptGrammar::T_TRUE;
        else if (c[0] == QLatin1Char('n') && c[1] == QLatin1Char('u')
                && c[2] == QLatin1Char('l') && c[3] == QLatin1Char('l'))
            return QScriptGrammar::T_NULL;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('e') && c[1] == QLatin1Char('n')
                    && c[2] == QLatin1Char('u') && c[3] == QLatin1Char('m'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('b') && c[1] == QLatin1Char('y')
                    && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('l') && c[1] == QLatin1Char('o')
                    && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('g'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('c') && c[1] == QLatin1Char('h')
                    && c[2] == QLatin1Char('a') && c[3] == QLatin1Char('r'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('g') && c[1] == QLatin1Char('o')
                    && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('o'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 5: {
        if (c[0] == QLatin1Char('b') && c[1] == QLatin1Char('r')
                && c[2] == QLatin1Char('e') && c[3] == QLatin1Char('a')
                && c[4] == QLatin1Char('k'))
            return QScriptGrammar::T_BREAK;
        else if (c[0] == QLatin1Char('c') && c[1] == QLatin1Char('a')
                && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('c')
                && c[4] == QLatin1Char('h'))
            return QScriptGrammar::T_CATCH;
        else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('h')
                && c[2] == QLatin1Char('r') && c[3] == QLatin1Char('o')
                && c[4] == QLatin1Char('w'))
            return QScriptGrammar::T_THROW;
        else if (c[0] == QLatin1Char('w') && c[1] == QLatin1Char('h')
                && c[2] == QLatin1Char('i') && c[3] == QLatin1Char('l')
                && c[4] == QLatin1Char('e'))
            return QScriptGrammar::T_WHILE;
        else if (c[0] == QLatin1Char('c') && c[1] == QLatin1Char('o')
                && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('s')
                && c[4] == QLatin1Char('t'))
            return QScriptGrammar::T_CONST;
        else if (c[0] == QLatin1Char('f') && c[1] == QLatin1Char('a')
                && c[2] == QLatin1Char('l') && c[3] == QLatin1Char('s')
                && c[4] == QLatin1Char('e'))
            return QScriptGrammar::T_FALSE;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('s') && c[1] == QLatin1Char('h')
                    && c[2] == QLatin1Char('o') && c[3] == QLatin1Char('r')
                    && c[4] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('s') && c[1] == QLatin1Char('u')
                    && c[2] == QLatin1Char('p') && c[3] == QLatin1Char('e')
                    && c[4] == QLatin1Char('r'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('f') && c[1] == QLatin1Char('i')
                    && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('a')
                    && c[4] == QLatin1Char('l'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('c') && c[1] == QLatin1Char('l')
                    && c[2] == QLatin1Char('a') && c[3] == QLatin1Char('s')
                    && c[4] == QLatin1Char('s'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('f') && c[1] == QLatin1Char('l')
                    && c[2] == QLatin1Char('o') && c[3] == QLatin1Char('a')
                    && c[4] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 6: {
        if (c[0] == QLatin1Char('d') && c[1] == QLatin1Char('e')
                && c[2] == QLatin1Char('l') && c[3] == QLatin1Char('e')
                && c[4] == QLatin1Char('t') && c[5] == QLatin1Char('e'))
            return QScriptGrammar::T_DELETE;
        else if (c[0] == QLatin1Char('r') && c[1] == QLatin1Char('e')
                && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('u')
                && c[4] == QLatin1Char('r') && c[5] == QLatin1Char('n'))
            return QScriptGrammar::T_RETURN;
        else if (c[0] == QLatin1Char('s') && c[1] == QLatin1Char('w')
                && c[2] == QLatin1Char('i') && c[3] == QLatin1Char('t')
                && c[4] == QLatin1Char('c') && c[5] == QLatin1Char('h'))
            return QScriptGrammar::T_SWITCH;
        else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('y')
                && c[2] == QLatin1Char('p') && c[3] == QLatin1Char('e')
                && c[4] == QLatin1Char('o') && c[5] == QLatin1Char('f'))
            return QScriptGrammar::T_TYPEOF;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('e') && c[1] == QLatin1Char('x')
                    && c[2] == QLatin1Char('p') && c[3] == QLatin1Char('o')
                    && c[4] == QLatin1Char('r') && c[5] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('s') && c[1] == QLatin1Char('t')
                    && c[2] == QLatin1Char('a') && c[3] == QLatin1Char('t')
                    && c[4] == QLatin1Char('i') && c[5] == QLatin1Char('c'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('d') && c[1] == QLatin1Char('o')
                    && c[2] == QLatin1Char('u') && c[3] == QLatin1Char('b')
                    && c[4] == QLatin1Char('l') && c[5] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('m')
                    && c[2] == QLatin1Char('p') && c[3] == QLatin1Char('o')
                    && c[4] == QLatin1Char('r') && c[5] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('p') && c[1] == QLatin1Char('u')
                    && c[2] == QLatin1Char('b') && c[3] == QLatin1Char('l')
                    && c[4] == QLatin1Char('i') && c[5] == QLatin1Char('c'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('n') && c[1] == QLatin1Char('a')
                    && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('i')
                    && c[4] == QLatin1Char('v') && c[5] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('h')
                    && c[2] == QLatin1Char('r') && c[3] == QLatin1Char('o')
                    && c[4] == QLatin1Char('w') && c[5] == QLatin1Char('s'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 7: {
        if (c[0] == QLatin1Char('d') && c[1] == QLatin1Char('e')
                && c[2] == QLatin1Char('f') && c[3] == QLatin1Char('a')
                && c[4] == QLatin1Char('u') && c[5] == QLatin1Char('l')
                && c[6] == QLatin1Char('t'))
            return QScriptGrammar::T_DEFAULT;
        else if (c[0] == QLatin1Char('f') && c[1] == QLatin1Char('i')
                && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('a')
                && c[4] == QLatin1Char('l') && c[5] == QLatin1Char('l')
                && c[6] == QLatin1Char('y'))
            return QScriptGrammar::T_FINALLY;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('b') && c[1] == QLatin1Char('o')
                    && c[2] == QLatin1Char('o') && c[3] == QLatin1Char('l')
                    && c[4] == QLatin1Char('e') && c[5] == QLatin1Char('a')
                    && c[6] == QLatin1Char('n'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('e') && c[1] == QLatin1Char('x')
                    && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('e')
                    && c[4] == QLatin1Char('n') && c[5] == QLatin1Char('d')
                    && c[6] == QLatin1Char('s'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('p') && c[1] == QLatin1Char('a')
                    && c[2] == QLatin1Char('c') && c[3] == QLatin1Char('k')
                    && c[4] == QLatin1Char('a') && c[5] == QLatin1Char('g')
                    && c[6] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('p') && c[1] == QLatin1Char('r')
                    && c[2] == QLatin1Char('i') && c[3] == QLatin1Char('v')
                    && c[4] == QLatin1Char('a') && c[5] == QLatin1Char('t')
                    && c[6] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 8: {
        if (c[0] == QLatin1Char('c') && c[1] == QLatin1Char('o')
                && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('t')
                && c[4] == QLatin1Char('i') && c[5] == QLatin1Char('n')
                && c[6] == QLatin1Char('u') && c[7] == QLatin1Char('e'))
            return QScriptGrammar::T_CONTINUE;
        else if (c[0] == QLatin1Char('f') && c[1] == QLatin1Char('u')
                && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('c')
                && c[4] == QLatin1Char('t') && c[5] == QLatin1Char('i')
                && c[6] == QLatin1Char('o') && c[7] == QLatin1Char('n'))
            return QScriptGrammar::T_FUNCTION;
        else if (c[0] == QLatin1Char('d') && c[1] == QLatin1Char('e')
                && c[2] == QLatin1Char('b') && c[3] == QLatin1Char('u')
                && c[4] == QLatin1Char('g') && c[5] == QLatin1Char('g')
                && c[6] == QLatin1Char('e') && c[7] == QLatin1Char('r'))
            return QScriptGrammar::T_DEBUGGER;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('a') && c[1] == QLatin1Char('b')
                    && c[2] == QLatin1Char('s') && c[3] == QLatin1Char('t')
                    && c[4] == QLatin1Char('r') && c[5] == QLatin1Char('a')
                    && c[6] == QLatin1Char('c') && c[7] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('v') && c[1] == QLatin1Char('o')
                    && c[2] == QLatin1Char('l') && c[3] == QLatin1Char('a')
                    && c[4] == QLatin1Char('t') && c[5] == QLatin1Char('i')
                    && c[6] == QLatin1Char('l') && c[7] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 9: {
        if (check_reserved) {
            if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('n')
                    && c[2] == QLatin1Char('t') && c[3] == QLatin1Char('e')
                    && c[4] == QLatin1Char('r') && c[5] == QLatin1Char('f')
                    && c[6] == QLatin1Char('a') && c[7] == QLatin1Char('c')
                    && c[8] == QLatin1Char('e'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('t') && c[1] == QLatin1Char('r')
                    && c[2] == QLatin1Char('a') && c[3] == QLatin1Char('n')
                    && c[4] == QLatin1Char('s') && c[5] == QLatin1Char('i')
                    && c[6] == QLatin1Char('e') && c[7] == QLatin1Char('n')
                    && c[8] == QLatin1Char('t'))
                return QScriptGrammar::T_RESERVED_WORD;
            else if (c[0] == QLatin1Char('p') && c[1] == QLatin1Char('r')
                    && c[2] == QLatin1Char('o') && c[3] == QLatin1Char('t')
                    && c[4] == QLatin1Char('e') && c[5] == QLatin1Char('c')
                    && c[6] == QLatin1Char('t') && c[7] == QLatin1Char('e')
                    && c[8] == QLatin1Char('d'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 10: {
        if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('n')
                && c[2] == QLatin1Char('s') && c[3] == QLatin1Char('t')
                && c[4] == QLatin1Char('a') && c[5] == QLatin1Char('n')
                && c[6] == QLatin1Char('c') && c[7] == QLatin1Char('e')
                && c[8] == QLatin1Char('o') && c[9] == QLatin1Char('f'))
            return QScriptGrammar::T_INSTANCEOF;
        else if (check_reserved) {
            if (c[0] == QLatin1Char('i') && c[1] == QLatin1Char('m')
                    && c[2] == QLatin1Char('p') && c[3] == QLatin1Char('l')
                    && c[4] == QLatin1Char('e') && c[5] == QLatin1Char('m')
                    && c[6] == QLatin1Char('e') && c[7] == QLatin1Char('n')
                    && c[8] == QLatin1Char('t') && c[9] == QLatin1Char('s'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    case 12: {
        if (check_reserved) {
            if (c[0] == QLatin1Char('s') && c[1] == QLatin1Char('y')
                    && c[2] == QLatin1Char('n') && c[3] == QLatin1Char('c')
                    && c[4] == QLatin1Char('h') && c[5] == QLatin1Char('r')
                    && c[6] == QLatin1Char('o') && c[7] == QLatin1Char('n')
                    && c[8] == QLatin1Char('i') && c[9] == QLatin1Char('z')
                    && c[10] == QLatin1Char('e') && c[11] == QLatin1Char('d'))
                return QScriptGrammar::T_RESERVED_WORD;
        }
    }   break;

    } // switch

    return -1;
}

int QScript::Lexer::lex()
{
    int token = 0;
    state = Start;
    ushort stringType = 0; // either single or double quotes
    pos8 = pos16 = 0;
    done = false;
    terminator = false;

    // did we push a token on the stack previously ?
    // (after an automatic semicolon insertion)
    if (stackToken >= 0) {
        setDone(Other);
        token = stackToken;
        stackToken = -1;
    }

    while (!done) {
        switch (state) {
        case Start:
            if (isWhiteSpace()) {
                // do nothing
            } else if (current == '/' && next1 == '/') {
                recordStartPos();
                shift(1);
                state = InSingleLineComment;
            } else if (current == '/' && next1 == '*') {
                recordStartPos();
                shift(1);
                state = InMultiLineComment;
            } else if (current == 0) {
                syncProhibitAutomaticSemicolon();
                if (!terminator && !delimited && !prohibitAutomaticSemicolon) {
                    // automatic semicolon insertion if program incomplete
                    token = QScriptGrammar::T_SEMICOLON;
                    stackToken = 0;
                    setDone(Other);
                } else {
                    setDone(Eof);
                }
            } else if (isLineTerminator()) {
                shiftWindowsLineBreak();
                yylineno++;
                yycolumn = 0;
                bol = true;
                terminator = true;
                syncProhibitAutomaticSemicolon();
                if (restrKeyword) {
                    token = QScriptGrammar::T_SEMICOLON;
                    setDone(Other);
                }
            } else if (current == '"' || current == '\'') {
                recordStartPos();
                state = InString;
                stringType = current;
            } else if (isIdentLetter(current)) {
                recordStartPos();
                record16(current);
                state = InIdentifier;
            } else if (current == '0') {
                recordStartPos();
                record8(current);
                state = InNum0;
            } else if (isDecimalDigit(current)) {
                recordStartPos();
                record8(current);
                state = InNum;
            } else if (current == '.' && isDecimalDigit(next1)) {
                recordStartPos();
                record8(current);
                state = InDecimal;
            } else {
                recordStartPos();
                token = matchPunctuator(current, next1, next2, next3);
                if (token != -1) {
                    if (terminator && !delimited && !prohibitAutomaticSemicolon
                        && (token == QScriptGrammar::T_PLUS_PLUS
                            || token == QScriptGrammar::T_MINUS_MINUS)) {
                        // automatic semicolon insertion
                        stackToken = token;
                        token = QScriptGrammar::T_SEMICOLON;
                    }
                    setDone(Other);
                }
                else {
                    setDone(Bad);
                    err = IllegalCharacter;
                    errmsg = QLatin1String("Illegal character");
                }
            }
            break;
        case InString:
            if (current == stringType) {
                shift(1);
                setDone(String);
            } else if (current == 0 || isLineTerminator()) {
                setDone(Bad);
                err = UnclosedStringLiteral;
                errmsg = QLatin1String("Unclosed string at end of line");
            } else if (current == '\\') {
                state = InEscapeSequence;
            } else {
                record16(current);
            }
            break;
            // Escape Sequences inside of strings
        case InEscapeSequence:
            if (isOctalDigit(current)) {
                if (current >= '0' && current <= '3' &&
                     isOctalDigit(next1) && isOctalDigit(next2)) {
                    record16(convertOctal(current, next1, next2));
                    shift(2);
                    state = InString;
                } else if (isOctalDigit(current) &&
                            isOctalDigit(next1)) {
                    record16(convertOctal('0', current, next1));
                    shift(1);
                    state = InString;
                } else if (isOctalDigit(current)) {
                    record16(convertOctal('0', '0', current));
                    state = InString;
                } else {
                    setDone(Bad);
                    err = IllegalEscapeSequence;
                    errmsg = QLatin1String("Illegal escape squence");
                }
            } else if (current == 'x')
                state = InHexEscape;
            else if (current == 'u')
                state = InUnicodeEscape;
            else {
                if (isLineTerminator()) {
                    shiftWindowsLineBreak();
                    yylineno++;
                    yycolumn = 0;
                    bol = true;
                } else {
                    record16(singleEscape(current));
                }
                state = InString;
            }
            break;
        case InHexEscape:
            if (isHexDigit(current) && isHexDigit(next1)) {
                state = InString;
                record16(QLatin1Char(convertHex(current, next1)));
                shift(1);
            } else if (current == stringType) {
                record16(QLatin1Char('x'));
                shift(1);
                setDone(String);
            } else {
                record16(QLatin1Char('x'));
                record16(current);
                state = InString;
            }
            break;
        case InUnicodeEscape:
            if (isHexDigit(current) && isHexDigit(next1) &&
                 isHexDigit(next2) && isHexDigit(next3)) {
                record16(convertUnicode(current, next1, next2, next3));
                shift(3);
                state = InString;
            } else if (current == stringType) {
                record16(QLatin1Char('u'));
                shift(1);
                setDone(String);
            } else {
                setDone(Bad);
                err = IllegalUnicodeEscapeSequence;
                errmsg = QLatin1String("Illegal unicode escape sequence");
            }
            break;
        case InSingleLineComment:
            if (isLineTerminator()) {
                shiftWindowsLineBreak();
                yylineno++;
                yycolumn = 0;
                terminator = true;
                bol = true;
                if (restrKeyword) {
                    token = QScriptGrammar::T_SEMICOLON;
                    setDone(Other);
                } else
                    state = Start;
            } else if (current == 0) {
                setDone(Eof);
            }
            break;
        case InMultiLineComment:
            if (current == 0) {
                setDone(Bad);
                err = UnclosedComment;
                errmsg = QLatin1String("Unclosed comment at end of file");
            } else if (isLineTerminator()) {
                shiftWindowsLineBreak();
                yylineno++;
            } else if (current == '*' && next1 == '/') {
                state = Start;
                shift(1);
            }
            break;
        case InIdentifier:
            if (isIdentLetter(current) || isDecimalDigit(current)) {
                record16(current);
                break;
            }
            setDone(Identifier);
            break;
        case InNum0:
            if (current == 'x' || current == 'X') {
                record8(current);
                state = InHex;
            } else if (current == '.') {
                record8(current);
                state = InDecimal;
            } else if (current == 'e' || current == 'E') {
                record8(current);
                state = InExponentIndicator;
            } else if (isOctalDigit(current)) {
                record8(current);
                state = InOctal;
            } else if (isDecimalDigit(current)) {
                record8(current);
                state = InDecimal;
            } else {
                setDone(Number);
            }
            break;
        case InHex:
            if (isHexDigit(current))
                record8(current);
            else
                setDone(Hex);
            break;
        case InOctal:
            if (isOctalDigit(current)) {
                record8(current);
            } else if (isDecimalDigit(current)) {
                record8(current);
                state = InDecimal;
            } else {
                setDone(Octal);
            }
            break;
        case InNum:
            if (isDecimalDigit(current)) {
                record8(current);
            } else if (current == '.') {
                record8(current);
                state = InDecimal;
            } else if (current == 'e' || current == 'E') {
                record8(current);
                state = InExponentIndicator;
            } else {
                setDone(Number);
            }
            break;
        case InDecimal:
            if (isDecimalDigit(current)) {
                record8(current);
            } else if (current == 'e' || current == 'E') {
                record8(current);
                state = InExponentIndicator;
            } else {
                setDone(Number);
            }
            break;
        case InExponentIndicator:
            if (current == '+' || current == '-') {
                record8(current);
            } else if (isDecimalDigit(current)) {
                record8(current);
                state = InExponent;
            } else {
                setDone(Bad);
                err = IllegalExponentIndicator;
                errmsg = QLatin1String("Illegal syntax for exponential number");
            }
            break;
        case InExponent:
            if (isDecimalDigit(current)) {
                record8(current);
            } else {
                setDone(Number);
            }
            break;
        default:
            Q_ASSERT_X(0, "Lexer::lex", "Unhandled state in switch statement");
        }

        // move on to the next character
        if (!done)
            shift(1);
        if (state != Start && state != InSingleLineComment)
            bol = false;
    }

    // no identifiers allowed directly after numeric literal, e.g. "3in" is bad
    if ((state == Number || state == Octal || state == Hex)
         && isIdentLetter(current)) {
        state = Bad;
        err = IllegalIdentifier;
        errmsg = QLatin1String("Identifier cannot start with numeric literal");
    }

    // terminate string
    buffer8[pos8] = '\0';

    double dval = 0;
    if (state == Number) {
        dval = qstrtod(buffer8, 0, 0);
    } else if (state == Hex) { // scan hex numbers
        dval = QScript::integerFromString(buffer8, pos8, 16);
        state = Number;
    } else if (state == Octal) {   // scan octal number
        dval = QScript::integerFromString(buffer8, pos8, 8);
        state = Number;
    }

    restrKeyword = false;
    delimited = false;

    switch (parenthesesState) {
    case IgnoreParentheses:
        break;
    case CountParentheses:
        if (token == QScriptGrammar::T_RPAREN) {
            --parenthesesCount;
            if (parenthesesCount == 0)
                parenthesesState = BalancedParentheses;
        } else if (token == QScriptGrammar::T_LPAREN) {
            ++parenthesesCount;
        }
        break;
    case BalancedParentheses:
        parenthesesState = IgnoreParentheses;
        break;
    }

    switch (state) {
    case Eof:
        return 0;
    case Other:
        if(token == QScriptGrammar::T_RBRACE || token == QScriptGrammar::T_SEMICOLON)
            delimited = true;
        return token;
    case Identifier:
        if ((token = findReservedWord(buffer16, pos16)) < 0) {
            /* TODO: close leak on parse error. same holds true for String */
            if (driver)
                qsyylval.ustr = driver->intern(buffer16, pos16);
            else
                qsyylval.ustr = 0;
            return QScriptGrammar::T_IDENTIFIER;
        }
        if (token == QScriptGrammar::T_CONTINUE || token == QScriptGrammar::T_BREAK
            || token == QScriptGrammar::T_RETURN || token == QScriptGrammar::T_THROW) {
            restrKeyword = true;
        } else if (token == QScriptGrammar::T_IF || token == QScriptGrammar::T_FOR
                   || token == QScriptGrammar::T_WHILE || token == QScriptGrammar::T_WITH) {
            parenthesesState = CountParentheses;
            parenthesesCount = 0;
        } else if (token == QScriptGrammar::T_DO) {
            parenthesesState = BalancedParentheses;
        }
        return token;
    case String:
        if (driver)
            qsyylval.ustr = driver->intern(buffer16, pos16);
        else
            qsyylval.ustr = 0;
        return QScriptGrammar::T_STRING_LITERAL;
    case Number:
        qsyylval.dval = dval;
        return QScriptGrammar::T_NUMERIC_LITERAL;
    case Bad:
        return -1;
    default:
        Q_ASSERT(!"unhandled numeration value in switch");
        return -1;
    }
}

bool QScript::Lexer::isWhiteSpace() const
{
    return (current == ' ' || current == '\t' ||
             current == 0x0b || current == 0x0c);
}

bool QScript::Lexer::isLineTerminator() const
{
    return (current == '\n' || current == '\r');
}

bool QScript::Lexer::isIdentLetter(ushort c)
{
    /* TODO: allow other legitimate unicode chars */
    return ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z')
            || c == '$'
            || c == '_');
}

bool QScript::Lexer::isDecimalDigit(ushort c)
{
    return (c >= '0' && c <= '9');
}

bool QScript::Lexer::isHexDigit(ushort c) const
{
    return ((c >= '0' && c <= '9')
            || (c >= 'a' && c <= 'f')
            || (c >= 'A' && c <= 'F'));
}

bool QScript::Lexer::isOctalDigit(ushort c) const
{
    return (c >= '0' && c <= '7');
}

int QScript::Lexer::matchPunctuator(ushort c1, ushort c2,
                            ushort c3, ushort c4)
{
    if (c1 == '>' && c2 == '>' && c3 == '>' && c4 == '=') {
        shift(4);
        return QScriptGrammar::T_GT_GT_GT_EQ;
    } else if (c1 == '=' && c2 == '=' && c3 == '=') {
        shift(3);
        return QScriptGrammar::T_EQ_EQ_EQ;
    } else if (c1 == '!' && c2 == '=' && c3 == '=') {
        shift(3);
        return QScriptGrammar::T_NOT_EQ_EQ;
    } else if (c1 == '>' && c2 == '>' && c3 == '>') {
        shift(3);
        return QScriptGrammar::T_GT_GT_GT;
    } else if (c1 == '<' && c2 == '<' && c3 == '=') {
        shift(3);
        return QScriptGrammar::T_LT_LT_EQ;
    } else if (c1 == '>' && c2 == '>' && c3 == '=') {
        shift(3);
        return QScriptGrammar::T_GT_GT_EQ;
    } else if (c1 == '<' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_LE;
    } else if (c1 == '>' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_GE;
    } else if (c1 == '!' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_NOT_EQ;
    } else if (c1 == '+' && c2 == '+') {
        shift(2);
        return QScriptGrammar::T_PLUS_PLUS;
    } else if (c1 == '-' && c2 == '-') {
        shift(2);
        return QScriptGrammar::T_MINUS_MINUS;
    } else if (c1 == '=' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_EQ_EQ;
    } else if (c1 == '+' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_PLUS_EQ;
    } else if (c1 == '-' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_MINUS_EQ;
    } else if (c1 == '*' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_STAR_EQ;
    } else if (c1 == '/' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_DIVIDE_EQ;
    } else if (c1 == '&' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_AND_EQ;
    } else if (c1 == '^' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_XOR_EQ;
    } else if (c1 == '%' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_REMAINDER_EQ;
    } else if (c1 == '|' && c2 == '=') {
        shift(2);
        return QScriptGrammar::T_OR_EQ;
    } else if (c1 == '<' && c2 == '<') {
        shift(2);
        return QScriptGrammar::T_LT_LT;
    } else if (c1 == '>' && c2 == '>') {
        shift(2);
        return QScriptGrammar::T_GT_GT;
    } else if (c1 == '&' && c2 == '&') {
        shift(2);
        return QScriptGrammar::T_AND_AND;
    } else if (c1 == '|' && c2 == '|') {
        shift(2);
        return QScriptGrammar::T_OR_OR;
    }

    switch(c1) {
        case '=': shift(1); return QScriptGrammar::T_EQ;
        case '>': shift(1); return QScriptGrammar::T_GT;
        case '<': shift(1); return QScriptGrammar::T_LT;
        case ',': shift(1); return QScriptGrammar::T_COMMA;
        case '!': shift(1); return QScriptGrammar::T_NOT;
        case '~': shift(1); return QScriptGrammar::T_TILDE;
        case '?': shift(1); return QScriptGrammar::T_QUESTION;
        case ':': shift(1); return QScriptGrammar::T_COLON;
        case '.': shift(1); return QScriptGrammar::T_DOT;
        case '+': shift(1); return QScriptGrammar::T_PLUS;
        case '-': shift(1); return QScriptGrammar::T_MINUS;
        case '*': shift(1); return QScriptGrammar::T_STAR;
        case '/': shift(1); return QScriptGrammar::T_DIVIDE_;
        case '&': shift(1); return QScriptGrammar::T_AND;
        case '|': shift(1); return QScriptGrammar::T_OR;
        case '^': shift(1); return QScriptGrammar::T_XOR;
        case '%': shift(1); return QScriptGrammar::T_REMAINDER;
        case '(': shift(1); return QScriptGrammar::T_LPAREN;
        case ')': shift(1); return QScriptGrammar::T_RPAREN;
        case '{': shift(1); return QScriptGrammar::T_LBRACE;
        case '}': shift(1); return QScriptGrammar::T_RBRACE;
        case '[': shift(1); return QScriptGrammar::T_LBRACKET;
        case ']': shift(1); return QScriptGrammar::T_RBRACKET;
        case ';': shift(1); return QScriptGrammar::T_SEMICOLON;

        default: return -1;
    }
}

ushort QScript::Lexer::singleEscape(ushort c) const
{
    switch(c) {
    case 'b':
        return 0x08;
    case 't':
        return 0x09;
    case 'n':
        return 0x0A;
    case 'v':
        return 0x0B;
    case 'f':
        return 0x0C;
    case 'r':
        return 0x0D;
    case '"':
        return 0x22;
    case '\'':
        return 0x27;
    case '\\':
        return 0x5C;
    default:
        return c;
    }
}

ushort QScript::Lexer::convertOctal(ushort c1, ushort c2,
                            ushort c3) const
{
    return ((c1 - '0') * 64 + (c2 - '0') * 8 + c3 - '0');
}

unsigned char QScript::Lexer::convertHex(ushort c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    else if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    else
        return (c - 'A' + 10);
}

unsigned char QScript::Lexer::convertHex(ushort c1, ushort c2)
{
    return ((convertHex(c1) << 4) + convertHex(c2));
}

QChar QScript::Lexer::convertUnicode(ushort c1, ushort c2,
                             ushort c3, ushort c4)
{
    return QChar((convertHex(c3) << 4) + convertHex(c4),
                  (convertHex(c1) << 4) + convertHex(c2));
}

void QScript::Lexer::record8(ushort c)
{
    Q_ASSERT(c <= 0xff);

    // enlarge buffer if full
    if (pos8 >= size8 - 1) {
        char *tmp = new char[2 * size8];
        memcpy(tmp, buffer8, size8 * sizeof(char));
        delete [] buffer8;
        buffer8 = tmp;
        size8 *= 2;
    }

    buffer8[pos8++] = (char) c;
}

void QScript::Lexer::record16(QChar c)
{
    // enlarge buffer if full
    if (pos16 >= size16 - 1) {
        QChar *tmp = new QChar[2 * size16];
        memcpy(tmp, buffer16, size16 * sizeof(QChar));
        delete [] buffer16;
        buffer16 = tmp;
        size16 *= 2;
    }

    buffer16[pos16++] = c;
}

void QScript::Lexer::recordStartPos()
{
    startlineno = yylineno;
    startcolumn = yycolumn;
}

bool QScript::Lexer::scanRegExp(RegExpBodyPrefix prefix)
{
    pos16 = 0;
    bool lastWasEscape = false;

    if (prefix == EqualPrefix)
        record16(QLatin1Char('='));

    while (1) {
        if (isLineTerminator() || current == 0) {
            errmsg = QLatin1String("Unterminated regular expression literal");
            return false;
        }
        else if (current != '/' || lastWasEscape == true)
            {
                record16(current);
                lastWasEscape = !lastWasEscape && (current == '\\');
            }
        else {
            if (driver)
                pattern = driver->intern(buffer16, pos16);
            else
                pattern = 0;
            pos16 = 0;
            shift(1);
            break;
        }
        shift(1);
    }

    flags = 0;
    while (isIdentLetter(current)) {
        int flag = QScript::Ecma::RegExp::flagFromChar(current);
        if (flag == 0) {
            errmsg = QString::fromLatin1("Invalid regular expression flag '%0'")
                     .arg(QChar(current));
            return false;
        }
        flags |= flag;
        record16(current);
        shift(1);
    }

    return true;
}

void QScript::Lexer::syncProhibitAutomaticSemicolon()
{
    if (parenthesesState == BalancedParentheses) {
        // we have seen something like "if (foo)", which means we should
        // never insert an automatic semicolon at this point, since it would
        // then be expanded into an empty statement (ECMA-262 7.9.1)
        prohibitAutomaticSemicolon = true;
        parenthesesState = IgnoreParentheses;
    } else {
        prohibitAutomaticSemicolon = false;
    }
}

QT_END_NAMESPACE

