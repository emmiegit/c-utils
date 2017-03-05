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

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char *);

/* Enable parser debugging */
#if 0
# define YYDEBUG 1
extern int yydebug;
int yydebug = 1;
#endif
%}

%union {
    double num;
}

/* Precedence order */
%nonassoc '\n' NUMBER EXIT
%left '+' '-' '*' '/' '^' FLOORDIV LSHIFT RSHIFT AND OR XOR
%left ','
%left '(' ')' '[' ']' '{' '}'
%nonassoc ABS ACOS ASIN ATAN ATANH CBRT CEIL COS COSH DIM EXP EXP2 FLOOR GAMMA LOG LOG10 LOG2 LOGB MAX MIN MOD RINT ROUND RT SIN SINH SQRT TAN TANH TRUNC

/* Tokens */
%token NUMBER
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
%token EXIT
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
%token INVALID

%type<num> NUMBER
%type<num> expr

%start top

%%
top
        : expr                          { result = $1; }
        | EXIT                          { done = 1; YYABORT; }
        ;

expr
        : NUMBER                        { $$ = $1; }
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
        | DIM expr ',' expr             { $$ = fdim($2, $4); }
        | EXP expr                      { $$ = exp($2); }
        | EXP2 expr                     { $$ = exp2($2); }
        | FLOOR expr                    { $$ = floor($2); }
        | GAMMA expr                    { $$ = tgamma($2); }
        | LOG expr                      { $$ = log($2); }
        | LOG10 expr                    { $$ = log10($2); }
        | LOG2 expr                     { $$ = log2($2); }
        | LOGB expr                     { $$ = logb($2); }
        | MAX expr ',' expr             { $$ = fmax($2, $4); }
        | MIN expr ',' expr             { $$ = fmin($2, $4); }
        | RINT expr                     { $$ = rint($2); }
        | RT expr ',' expr              { $$ = pow($4, 1 / $2); }
        | ROUND expr                    { $$ = round($2); }
        | SIN expr                      { $$ = sin($2); }
        | SINH expr                     { $$ = sinh($2); }
        | SQRT expr                     { $$ = sqrt($2); }
        | TAN expr                      { $$ = tan($2); }
        | TANH expr                     { $$ = tanh($2); }
        | TRUNC expr                    { $$ = trunc($2); }
        ;
%%
