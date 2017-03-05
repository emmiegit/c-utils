/*
 * calc.c
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

#include "y.tab.h"

#include "calc.h"

/* Externals */
struct file_location yy_location;
const char *yyin_filename;

int calc_file(const char *fn, FILE *fh)
{
	yyin_filename = fn;
	yyin = fh;

	if (yyparse())
		return 1;
	return 0;
}

void print_result(double num)
{
	if (num == (long)num)
		printf("= %ld\n", (long)num);
	else
		printf("= %f\n", num);
}