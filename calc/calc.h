/*
 * calc.h
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

#ifndef _CALC_H_
#define _CALC_H_

#include <stdio.h>

struct file_location {
	unsigned int first_line, first_column;
	unsigned int last_line, last_column;
};

/*
 * Read and parse through the given file,
 * evaluating the expression it comes across.
 */
int calc_file(const char *fn, FILE *fh);

void print_result(double num);

/* Externals */
extern struct file_location yy_location;
extern const char *yyin_filename;
extern FILE *yyin;

#endif /* _CALC_H_ */

