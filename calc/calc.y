/*
 * calc.y
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
#define YYERROR_VERBOSE

#include <math.h>
#include <stdio.h>

#include "calc.h"
#include "main.h"
#include "variables.h"

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char *);

/* Enable parser debugging */
#if 0
# define YYDEBUG 1
extern int yydebug;
int yydebug = 1;
#endif

#define PAIR_X(p)                   ((p).x)
#define PAIR_Y(p)                   ((p).y)

#define ASSIGN_PAIR(p, _x, _y)      \
    do {                            \
        (p).x = (_x);               \
        (p).y = (_y);               \
    } while (0)

#define GET_VARIABLE(x, v)          \
    do {                            \
        if (var_get(&(v), &(x)))    \
            YYABORT;                \
    } while (0)

#define TERMINATE()                 \
    do {                            \
        result.running = 0;         \
        YYABORT;                    \
    } while (0)

#define FINISH()                    \
    do {                            \
        result.has_ans = 0;         \
    } while (0)

#define FINISH_ANSWER(x)            \
    do {                            \
        result.has_ans = 1;         \
        result.answer = (x);        \
    } while (0)

#define FINISH_VARIABLE(v, x)       \
    do {                            \
        result.has_ans = 0;         \
        if (var_set(&(v), (x)))     \
            YYABORT;                \
    } while (0)
%}

%union {
    double val;
    struct pair pair;
    struct str str;
}

/* Precedence order */
%nonassoc '~' NUMBER VARIABLE LIST_VARS EXIT
%left '+' '-' '*' '/' '^' FLOORDIV LSHIFT RSHIFT AND OR XOR
%left ','
%left '(' ')' '[' ']' '{' '}'
%nonassoc ABS ACOS ASIN ATAN ATANH CBRT CEIL COS COSH DIM EXP EXP2 FLOOR GAMMA LOG LOG10 LOG2 LOGB MAX MIN MOD RINT ROUND RT SIN SINH SQRT TAN TANH TRUNC

/* Tokens */
%token NUMBER
%token VARIABLE
%token LIST_VARS
%token HELP
%token EXIT
%token AND
%token OR
%token XOR
%token FLOORDIV
%token LSHIFT
%token RSHIFT
%token ABS
%token ACOS
%token ASIN
%token ATAN
%token ATANH
%token CBRT
%token CEIL
%token COS
%token COSH
%token DIM
%token EXP
%token EXP2
%token FLOOR
%token GAMMA
%token LOG
%token LOG10
%token LOG2
%token LOGB
%token MAX
%token MIN
%token MOD
%token RINT
%token ROUND
%token RT
%token SIN
%token SINH
%token SQRT
%token TAN
%token TANH
%token TRUNC

%type<val> NUMBER
%type<str> VARIABLE
%type<val> expr
%type<pair> pair

%start top

%%
top
        : expr                          { FINISH_ANSWER($1); }
        | VARIABLE '=' expr             { FINISH_VARIABLE($1, $3); }
        | LIST_VARS                     { FINISH(); var_list(); }
        | HELP                          { FINISH(); print_help(); }
        | EXIT                          { FINISH(); TERMINATE(); }
        ;

expr
        : NUMBER                        { $$ = $1; }
        | VARIABLE                      { GET_VARIABLE($$, $1); }
        | '+' expr %prec NUMBER         { $$ = +$2; }
        | '-' expr %prec NUMBER         { $$ = -$2; }
        | '~' expr %prec NUMBER         { $$ = ~(long)$2; }
        | '(' expr ')'                  { $$ = $2; }
        | '[' expr ']'                  { $$ = $2; }
        | '{' expr '}'                  { $$ = $2; }
        | expr '+' expr                 { $$ = $1 + $3; }
        | expr '-' expr                 { $$ = $1 - $3; }
        | expr '*' expr                 { $$ = $1 * $3; }
        | expr '/' expr                 { $$ = $1 / $3; }
        | expr '^' expr                 { $$ = pow($1, $3); }
        | expr MOD expr                 { $$ = fmod($1, $3); }
        | expr FLOORDIV expr            { $$ = floor($1 / $3); }
        | expr LSHIFT expr              { $$ = (long)$1 << (long)$3; }
        | expr RSHIFT expr              { $$ = (long)$1 >> (long)$3; }
        | expr AND expr                 { $$ = (long)$1 & (long)$3; }
        | expr OR expr                  { $$ = (long)$1 | (long)$3; }
        | expr XOR expr                 { $$ = (long)$1 ^ (long)$3; }
        | ABS expr                      { $$ = fabs($2); }
        | ACOS expr                     { $$ = acos($2); }
        | ASIN expr                     { $$ = asin($2); }
        | ATAN expr                     { $$ = atan($2); }
        | ATANH expr                    { $$ = atanh($2); }
        | CBRT expr                     { $$ = cbrt($2); }
        | CEIL expr                     { $$ = ceil($2); }
        | COS expr                      { $$ = cos($2); }
        | COSH expr                     { $$ = cosh($2); }
        | DIM pair                      { $$ = fdim(PAIR_X($2), PAIR_Y($2)); }
        | EXP expr                      { $$ = exp($2); }
        | EXP2 expr                     { $$ = exp2($2); }
        | FLOOR expr                    { $$ = floor($2); }
        | GAMMA expr                    { $$ = tgamma($2); }
        | LOG expr                      { $$ = log($2); }
        | LOG10 expr                    { $$ = log10($2); }
        | LOG2 expr                     { $$ = log2($2); }
        | LOGB expr                     { $$ = logb($2); }
        | MAX pair                      { $$ = fmax(PAIR_X($2), PAIR_Y($2)); }
        | MIN pair                      { $$ = fmin(PAIR_X($2), PAIR_Y($2)); }
        | RINT expr                     { $$ = rint($2); }
        | RT pair                       { $$ = pow(PAIR_Y($2), 1 / PAIR_X($2)); }
        | ROUND expr                    { $$ = round($2); }
        | SIN expr                      { $$ = sin($2); }
        | SINH expr                     { $$ = sinh($2); }
        | SQRT expr                     { $$ = sqrt($2); }
        | TAN expr                      { $$ = tan($2); }
        | TANH expr                     { $$ = tanh($2); }
        | TRUNC expr                    { $$ = trunc($2); }
        ;

pair
        : expr ',' expr                 { ASSIGN_PAIR($$, $1, $3); }
        | '(' expr ',' expr ')'         { ASSIGN_PAIR($$, $2, $4); }
        | '[' expr ',' expr ']'         { ASSIGN_PAIR($$, $2, $4); }
        | '{' expr ',' expr '}'         { ASSIGN_PAIR($$, $2, $4); }
        ;
%%
