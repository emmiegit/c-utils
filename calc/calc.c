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

#include <readline/readline.h>
#include <readline/history.h>

#include "y.tab.h"

#include "calc.h"

/* Externals */
const char *yyin_filename;
double result;
double last;
int done;

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

static const char *get_line(const char *prompt)
{
	static char *line;

	if (line)
		free(line);
	line = readline(prompt);
	if (line && line[0])
		add_history(line);
	return line;
}

void execute_file(const char *fn)
{
	yyin_filename = fn;

	done = 0;
	while (!done) {
		const char *line;

		line = get_line("> ");
		if (!line)
			return;

		lex_new(line);
		if (!yyparse())
			print_result(result);
		lex_del();
	}
}
