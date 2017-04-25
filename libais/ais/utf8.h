/*
 * utf8.h
 *
 * libais - Ammon's C library
 * Copyright (c) 2016 Ammon Smith
 *
 * libais is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version
 * 2 of the License, or (at your option) any later version.
 *
 * libais is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with libais.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __AIS_UTF8_H
#define __AIS_UTF8_H

#include <stdint.h>

/*
 * Converts the given code point described
 * by "codept" to a UTF-8 byte sequence in
 * "buf". It returns the number of bytes
 * written to the buffer, or -1 if an error
 * occurs.
 *
 * "buf" point to a writeable region at least
 * four bytes in length.
 */
int encode_utf8(int32_t codept, char *buf);

/*
 * Decodes the given byte sequence into a
 * Unicode codepoint. You pass a pointer "buf"
 * "len" bytes long.
 *
 * The number bytes read from the array is
 * returned, or -1 if the byte sequence is
 * invalid.
 */
int decode_utf8(int32_t *codept, const char *buf, unsigned int len);

#endif /* __AIS_UTF8_H */

