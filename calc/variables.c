/*
 * variables.c
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
#include <string.h>

#include "main.h"
#include "variables.h"

#define MAX_VARIABLES   512u

#define GET_ENTRY(x)	(&(table.entries[(x) % ARRAY_SIZE(table.entries)]))

/* Statically-sized hash table with linear probing */
static struct {
	struct entry {
		char *key;
		double value;
	} entries[MAX_VARIABLES];
	size_t size;
} table;

static size_t get_index(const struct str *str)
{
	unsigned long hash;
	size_t i;

	hash = 5381;
	for (i = 0; i < str->len; i++)
		hash = ((hash << 5) + hash) + str->ptr[i];
	return hash % ARRAY_SIZE(table.entries);
}

static char *string_dup(const struct str *s)
{
	char *buf;

	buf = malloc(s->len + 1);
	if (!buf)
		return NULL;
	memcpy(buf, s->ptr, s->len);
	buf[s->len] = '\0';
	return buf;
}

static int strings_equal(const struct str *x, const char *y)
{
	size_t i;

	for (i = 0; i < x->len; i++) {
		/* the other string ends early */
		if (!y[0])
			return 0;

		/* the strings are inequal */
		if (x->ptr[0] != y[0])
			return 0;
	}
	return 1;
}

static void string_put(const struct str *s, FILE *out)
{
	size_t i;

	for (i = 0; i < s->len; i++)
		putc(s->ptr[i], out);
}

int var_set(const struct str *var, double val)
{
	size_t index, i;

	if (table.size >= MAX_VARIABLES) {
		fprintf(stderr, "%s: only %u variables are supported\n",
			PROGRAM_NAME, MAX_VARIABLES);
		return -1;
	}

	index = get_index(var);
	for (i = 0; i < ARRAY_SIZE(table.entries); i++) {
		struct entry *ent;

		ent = GET_ENTRY(index + i);
		if (!ent->key) {
			char *str;

			str = string_dup(var);
			if (!str) {
				fprintf(stderr, "%s: unable to duplicate string\n",
					PROGRAM_NAME);
				return -1;
			}

			ent->key = str;
			ent->value = val;
			table.size++;
			return 0;
		}
		if (strings_equal(var, ent->key)) {
			ent->value = val;
			return 0;
		}
	}
	abort();
}

int var_get(const struct str *var, double *val)
{
	size_t index, i;

	index = get_index(var);
	for (i = 0; i < ARRAY_SIZE(table.entries); i++) {
		const struct entry *ent;

		ent = GET_ENTRY(index + i);
		if (!ent->key)
			break;
		if (strings_equal(var, ent->key)) {
			*val = ent->value;
			return 0;
		}
	}
	fprintf(stderr, "%s: no such variable: '", PROGRAM_NAME);
	string_put(var, stderr);
	fputs("'\n", stderr);
	return -1;
}

void var_list(void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(table.entries); i++) {
		const struct entry *ent;

		/*
		 * We don't used GET_ENTRY() because
		 * there's no need to roll over indices.
		 */
		ent = &table.entries[i];
		if (ent->key)
			printf("%s -> %f\n", ent->key, ent->value);
	}
}
