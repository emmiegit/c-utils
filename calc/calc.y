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
%nonassoc ABS ACOS ASIN ATAN ATANH CBRT CEIL COS COSH EXP EXP2 FLOOR
%left '+' '-' '*' '/' '^' FLOORDIV
%left '(' ')' '[' ']' '{' '}'

/* Tokens */
%token NUMBER
%token FLOORDIV
%token ABS
%token ACOS
%token ASIN
%token ATAN
%token ATANH
%token CBRT
%token CEIL
%token COS
%token COSH
%token EXP
%token EXP2
%token FLOOR
%token INVALID

%type<num> NUMBER
%type<num> expr

%start top

%%
top
        : expr                          { result = $1; }
        ;

expr
        : NUMBER                        { $$ = $1; }
        | '+' expr %prec NUMBER         { $$ = +$2; }
        | '-' expr %prec NUMBER         { $$ = -$2; }
        | '(' expr ')'                  { $$ = $2; }
        | '[' expr ']'                  { $$ = $2; }
        | '{' expr '}'                  { $$ = $2; }
        | expr '+' expr                 { $$ = $1 + $3; }
        | expr '-' expr                 { $$ = $1 - $3; }
        | expr '*' expr                 { $$ = $1 * $3; }
        | expr '/' expr                 { $$ = $1 / $3; }
        | expr '^' expr                 { $$ = pow($1, $3); }
        | expr FLOORDIV expr            { $$ = floor($1 / $3); }
        | ABS expr                      { $$ = fabs($2); }
        | ACOS expr                     { $$ = acos($2); }
        | ASIN expr                     { $$ = asin($2); }
        | ATAN expr                     { $$ = atan($2); }
        | ATANH expr                    { $$ = atanh($2); }
        | CBRT expr                     { $$ = cbrt($2); }
        | CEIL expr                     { $$ = ceil($2); }
        | COS expr                      { $$ = cos($2); }
        | COSH expr                     { $$ = cosh($2); }
        | EXP expr                      { $$ = exp($2); }
        | EXP2 expr                     { $$ = exp2($2); }
        | FLOOR expr                    { $$ = floor($2); }
        ;
%%
