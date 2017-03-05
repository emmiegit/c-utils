/*
 * main.h
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

#ifndef _MAIN_H_
#define _MAIN_H_

#define PROGRAM_NAME				"calc"
#define PROGRAM_VERSION_MAJOR			0
#define PROGRAM_VERSION_MINOR			4
#define PROGRAM_VERSION_PATCH			0

#ifndef GIT_HASH
# define GIT_HASH                               "nogithash"
#endif /* GIT_HASH */

#if defined(__clang__)
# define COMPILER_NAME                          "Clang/LLVM"
# define COMPILER_VERSION                       __clang_version__
#elif defined(__ICC) || defined(__INTEL_COMPILER)
# define COMPILER_NAME                          "Intel ICC"
# define COMPILER_VERSION                       __INTEL_COMPILER
#elif defined(__MINGW32__)
# define COMPILER_NAME                          "Mingw"
# define COMPILER_VERSION                       __VERSION__
#elif defined(__GNUC__) || defined(__GNUG__)
# define COMPILER_NAME                          "GCC"
# define COMPILER_VERSION                       __VERSION__
#elif defined(__HP_cc) || defined(__HP_aCC)
# define COMPILER_NAME                          "Hewlett-Packard C"
# define COMPILER_VERSION                       __HP_cc
#elif defined(__IBMC__) || defined(__IBMCPP__)
# define COMPILER_NAME                          "IBM XL C"
# define COMPILER_VERSION                       __xlc__
#elif defined(_MSC_VER)
# define COMPILER_NAME                          "Microsoft Visual Studio"
# define COMPILER_VERSION                       _MSC_VER
#elif defined(__PGI)
# define COMPILER_NAME                          "Portland Group PGCC"
# define COMPILER_VERSION                       __PGIC__
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
# define COMPILER_NAME                          "Oracle Solaris Studio"
# define COMPILER_VERSION                       __SUNPRO_C
#else
# define COMPILER_NAME                          "Unknown"
# define COMPILER_VERSION                       "compiler"
#endif

#if defined(_WIN64)
# define PLATFORM_NAME                          "Windows 64-bit"
#elif defined(_WIN32)
# define PLATFORM_NAME                          "Windows 32-bit"
#elif defined(__linux__)
# define PLATFORM_NAME                          "Linux"
#elif defined(__APPLE__) || defined(__MACH__)
# define PLATFORM_NAME                          "Apple"
#elif defined(__FreeBSD__)
# define PLATFORM_NAME                          "FreeBSD"
#elif defined(__unix) || defined(__unix__)
# define PLATFORM_NAME                          "Unknown UNIX"
#else
# define PLATFORM_NAME                          "Unknown"
#endif

/* Inline macros */
#define UNUSED(x)				((void)(x))
#define ARRAY_SIZE(x)				((sizeof(x)) / (sizeof((x)[0])))

void print_version(void);

#endif /* _MAIN_H_ */

