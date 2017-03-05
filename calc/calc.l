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

#include "calc.h"
#include "input.h"
#include "main.h"

#include "y.tab.h"

static double parse_num(const char *str);

#define YY_USER_ACTION                                      \
    yy_location.line = yylineno;                            \
    yy_location.first_column = yy_location.last_column;     \
    yy_location.last_column += yyleng;

#define SET_STRING(dest)                                    \
    do {                                                    \
        (dest).ptr = yytext;                                \
        (dest).len = yyleng;                                \
    } while (0)
%}

%option stack yylineno

NUM             ([0-9]+\.?[0-9]*([eE][-+]?[0-9]+)?)
HEX             (0[xX][0-9a-fA-F]+)
INF             ([iI][nN][fF]([iI][nN][iI][tT][yY])?)
NAN             ([nN][aA][nN])
LOG             ([lL][oO][gG])
LN              ([lL][nN])
VAR             ([A-Za-z_][A-Za-z_0-9]*)
EXIT            ([qQ]([uU][iI][tT])?|[eE][xX][iI][tT])
VARS            ([vV][aA][rR][sS])
HELP            ([hH][eE][lL][pP])

%%
[ \t\r\n]+      /* ignore whitespace */
{EXIT}          { return EXIT; }
{VARS}          { return LIST_VARS; }
{HELP}          { return HELP; }
({NUM}|{HEX})   { yylval.val = parse_num(yytext); return NUMBER; }
{INF}           { yylval.val = INFINITY; return NUMBER; }
{NAN}           { yylval.val = NAN; return NUMBER; }
[pP][iI]        { yylval.val = M_PI; return NUMBER; }
[eE]            { yylval.val = M_E; return NUMBER; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"//"            { return FLOORDIV; }
("%"|"mod")     { return MOD; }
("^"|"**")      { return '^'; }
"<<"            { return LSHIFT; }
">>"            { return RSHIFT; }
("~"|"not")     { return '~'; }
("&"|"and")     { return AND; }
("|"|"or")      { return OR; }
("^^"|"xor")    { return XOR; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"{"             { return '{'; }
"}"             { return '}'; }
","             { return ','; }
"="             { return '='; }
"_"             { yylval.val = last; return NUMBER; }
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
{VAR}           { SET_STRING(yylval.str); return VARIABLE; }
.               { return yytext[0]; }
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
            PROGRAM_NAME,
            strerror(errno));
        return NAN;
    }
#if !defined(NDEBUG)
    if (*ptr || isnan(result))
        abort();
#endif
    return result;
}

void lex_new(const char *str)
{
    yy_location.last_column = 0;
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
    size_t i, len;

    len = yy_location.first_column + PROMPT_LEN;
    for (i = 0; i < len; i++)
        putc(' ', stderr);
    fputs("^\n", stderr);
    fprintf(stderr, "%s: line %d: %s\n",
            PROGRAM_NAME,
            yylineno,
            message);
}
