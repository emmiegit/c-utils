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
#include "main.h"

static double parse_num(const char *str);
%}

%option stack yylineno

EXIT            ([qQ]([uU][iI][tT])?|[eE][xX][iI][tT])
NUM             ([0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)
HEX             (0[xX][0-9a-fA-F]+)
INF             ([iI][nN][fF]([iI][nN][iI][tT][yY])?)
LOG             ([lL][oO][gG])
LN              ([lL][nN])

%%
#.*$            /* ignore comments */
[ \t\r\n]+      /* ignore whitespace */
{EXIT}          { return EXIT; }
({NUM}|{HEX})   { yylval.num = parse_num(yytext); return NUMBER; }
{INF}           { yylval.num = INFINITY; return NUMBER; }
[pP][iI]        { yylval.num = M_PI; return NUMBER; }
[eE]            { yylval.num = M_E; return NUMBER; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"//"            { return FLOORDIV; }
("%"|"mod")     { return MOD; }
("^"|"**")      { return '^'; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"{"             { return '{'; }
"}"             { return '}'; }
","             { return ','; }
"_"             { yylval.num = last; return NUMBER; }
"abs"           { return ABS; }
"acos"          { return ACOS; }
"asin"          { return ASIN; }
"atan"          { return ATAN; }
"atanh"         { return ATANH; }
"cbrt"          { return CBRT; }
"ceil"          { return CEIL; }
"cos"           { return COS; }
"cosh"          { return COSH; }
"dim"           { return DIM; }
"exp"           { return EXP; }
"exp2"          { return EXP2; }
"floor"         { return FLOOR; }
"gamma"         { return GAMMA; }
("log"|"ln")    { return LOG; }
"log10"         { return LOG10; }
"log2"          { return LOG2; }
"logb"          { return LOGB; }
"max"           { return MAX; }
"min"           { return MIN; }
"rint"          { return RINT; }
"round"         { return ROUND; }
"rt"            { return RT; }
"sin"           { return SIN; }
"sinh"          { return SINH; }
"sqrt"          { return SQRT; }
"tan"           { return TAN; }
"tanh"          { return TANH; }
"trunc"         { return TRUNC; }
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

void lex_new(const char *str)
{
	yy_scan_string(str);
}

void lex_del(void)
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
}

int yywrap(void)
{
    return 1;
}

void yyerror(const char *message)
{
    fprintf(stderr, "%s: %s:%d: %s\n",
        PROGRAM_NAME,
        yyin_filename,
        yylineno,
        message);
}
