/*
 * roman.c
 *
 * Copyright 2017 Ammon Smith
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ctype.h>
#include <string.h>

#include "roman.h"

static const struct digit_value {
	char ch;
	short value;
} digits[] = {
	{ 'I', 1 },
	{ 'V', 5 },
	{ 'X', 10 },
	{ 'L', 50 },
	{ 'C', 100 },
	{ 'D', 500 },
	{ 'M', 1000 },
	{ '\0', -1 }
};

static short get_digit(char ch)
{
	const struct digit_value *dv;

	ch = toupper(ch);
	dv = &digits[0];
	while (dv->ch) {
		if (dv->ch == ch) {
			return dv->value;
		}
		dv++;
	}
	return -1;
}

long roman2long(const char *str)
{
	return nroman2long(str, strlen(str));
}

long nroman2long(const char *str, size_t len)
{
	int neg;
	long sum, partial;
	short last, cur;
	size_t i;

	/* Ignore leading whitespace */
	for (i = 0; isspace(str[i]); i++) {
		if (i == len) {
			return 0;
		}
	}

	/* Set initial values and flags */
	sum = 0;
	if (str[0] == '-') {
		neg = 1;
		i++;
	} else {
		neg = 0;
	}
	last = get_digit(str[i++]);
	partial = last;
	if (last < 0) {
		return -1;
	}

	/* Read each character */
	for (; i < len; i++) {
		if (isspace(str[i])) {
			break;
		}

		cur = get_digit(str[i]);
		if (cur < 0) {
			return -1;
		}
		if (cur > last) {
			sum = cur - partial;
			partial = 0;
		} else {
			partial += cur;
			if (cur < last) {
				last = cur;
			}
		}
	}
	sum += partial;

	/* Ignore trailing whitespace */
	for (; i < len; i++) {
		if (!isspace(str[i])) {
			return -1;
		}
	}

	return (!neg) ? sum : -sum;
}

char *long2roman(long num)
{
	(void)num;
	/* TODO */
	return NULL;
}

int long2nroman(long num, char *buf, size_t len)
{
	(void)num;
	(void)buf;
	(void)len;
	/* TODO */
	return -1;
}

