/*
 * input.h
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

#ifndef _INPUT_H_
#define _INPUT_H_

void input_init(void);
const char *get_line(const char *prompt);

extern size_t prompt_len;

#endif /* _INPUT_H_ */

