/*
 * config.h
 *
 * latex-autocompile - Automatically recompile LaTeX sources and
 *                     refresh the MuPDF viewer
 * Copyright (c) 2016-2017 Ammon Smith
 *
 * latex-autocompile is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * latex-autocompile is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with latex-autocompile.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Constants */

#define DEFAULT_DIRECTORY	"."

#define COMPILE_COMMAND		"pdflatex"

#define VIEW_COMMAND		"mupdf"

#define SOURCE_EXTENSION	".tex"
#define SOURCE_EXTENSION_LEN	4

#define PDF_EXTENSION		".pdf"
#define PDF_EXTENSION_LEN	4

/* Settings */

#define USE_VFORK		1

#endif /* _CONFIG_H_ */

