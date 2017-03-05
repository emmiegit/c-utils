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

/*
 * Read and parse through the given file,
 * evaluating the expression it comes across.
 */
void execute_file(void);

struct location {
	unsigned int line;
	unsigned int first_column, last_column;
};

struct pair {
	double x;
	double y;
};

struct result {
	double answer;

	unsigned running : 1;
	unsigned has_ans : 1;
};

/* Externals */
extern struct location yy_location;
extern struct result result;
extern double last;

#endif /* _CALC_H_ */

