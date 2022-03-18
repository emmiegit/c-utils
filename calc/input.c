/*
 * input.c
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

#include <stdio.h>

#include "input.h"

#if defined(USE_READLINE)

#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

void input_init(void)
{
	rl_editing_mode = 0;
	rl_bind_key('\t', rl_insert);
}

const char *get_line(void)
{
	static char *line;

	if (line) {
		free(line);
	}
	line = readline(PROMPT);
	if (line && line[0]) {
		add_history(line);
	}
	return line;
}

#else

void input_init(void)
{
}

const char *get_line(void)
{
	static char line[4096];

	fputs(PROMPT, stdout);
	return fgets(line, sizeof(line), stdin);
}

#endif /* USE_READLINE */

