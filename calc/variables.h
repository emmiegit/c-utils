/*
 * variables.h
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

#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include "types.h"

int var_set(const struct str *var, double val);
int var_get(const struct str *var, double *val);
void var_list(void);

#endif /* _VARIABLES_H_ */

