/*
 * calc.l
 *
 * calc - a simple CLI calculator
 * Copyright (c) 2017 Ammon Smith
 *
 * calc is available free of charge under the terms of the MIT
 * License. You are free to redistribute and/or modify it under those
 * terms. It is distributed in the hopes that it will be useful, but
 * WITHOUT ANY WARRANTY. See the LICENSE file for more details.
 *
 */

%{
#define YY_NO_LEAKS
#define YY_DECL         int yylex(void)

#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"

#include "calc.h"

static double parse_num(const char *str);

#define YY_USER_ACTION                                  \
    yy_location.first_line = yylineno;                  \
    yy_location.last_line = yylineno;                   \
    yy_location.first_column = yy_location.last_column; \
    yy_location.last_column += yyleng;
%}

%option stack yylineno

NUM             ([0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)
HEX             (0[xX][0-9a-fA-F]+)
INF             ([iI][nN][fF]([iI][nN][iI][tT][yY])?)

%%
#.*$            /* ignore comments */
[ \t\r\n]+      /* ignore whitespace */
({NUM}|{HEX})   { yylval.num = parse_num(yytext); return NUMBER; }
{INF}           { yylval.num = INFINITY; return NUMBER; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"//"            { return FLOORDIV; }
("^"|"**")      { return '^'; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"{"             { return '{'; }
"}"             { return '}'; }
[pP][iI]        { yylval.num = M_PI; return NUMBER; }
"abs"           { return ABS; }
"acos"          { return ACOS; }
"asin"          { return ASIN; }
"atan"          { return ATAN; }
"atanh"         { return ATANH; }
"cbrt"          { return CBRT; }
"ceil"          { return CEIL; }
"cos"           { return COS; }
"cosh"          { return COSH; }
"exp"           { return EXP; }
"exp2"          { return EXP2; }
"floor"         { return FLOOR; }
.               { return INVALID; }
%%

static double parse_num(const char *str)
{
    double result;
    char *ptr;

    errno = 0;
    result = strtod(str, &ptr);
    if (errno) {
        fprintf(stderr,
            "%s: unable to parse number: %s\n",
            yyin_filename,
            strerror(errno));
        return NAN;
    }
    if (*ptr || isnan(result))
        abort();
    return result;
}

int yywrap(void)
{
    return 1;
}

void yyerror(const char *message)
{
    if (yy_location.first_column >= 0 && interactive) {
        size_t i, off, len;

        off = yy_location.first_column;
        len = yy_location.last_column - yy_location.first_column - 1;
        for (i = 0; i < off; i++)
            putc(' ', stderr);
        for (i = 0; i < len; i++)
            putc('~', stderr);
        fputs("^\n", stderr);
    }
    fprintf(stderr, "%s:%d: %s\n",
        yyin_filename,
        yy_location.first_line,
        message);
}
