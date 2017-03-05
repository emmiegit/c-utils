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
struct file_location y_location;
int interactive;

int calc_file(const char *fn, FILE *fh)
{
	yyin_filename = fn;
	yyin = fh;

	return 0;
}
