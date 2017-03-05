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

#include <stdlib.h>
#include <stdio.h>

#include "calc.h"
#include "input.h"

#include "y.tab.h"

/* Externals */
struct location yy_location;
struct result result;
double last;

/* Parser utilities */
void lex_new(const char *str);
void lex_del(void);

static void print_result(double num)
{
	if (num == (long)num)
		printf("= %ld\n", (long)num);
	else
		printf("= %f\n", num);
}

void execute_file(void)
{
	/* Setup */
	result.running = 1;
	last = 0.0;

	while (result.running) {
		const char *line;

		line = get_line("> ");
		if (!line) {
			putchar('\n');
			return;
		} else if (!line[0] || line[0] == '\n') {
			continue;
		}

		lex_new(line);
		if (yyparse()) {
			lex_del();
			continue;
		}

		if (result.has_ans) {
			print_result(result.answer);
			last = result.answer;
		}
		lex_del();
	}
}
