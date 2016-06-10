/*
 * rmln.c
 * rmln - Remove a file if it's a symbolic link or a hard link.
 *
 * Licensed under the Apache 2.0 License.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "rmln.h"

#define STREQUALS(x, y)     (strcmp((x), (y)) == 0)

/* Function declarations */
int lstat(const char *pathname, struct stat *buf);
static int unlink_path(const char *path, bool verbose);
static void print_usage(const char *program_name, FILE *fh);
static void print_not_a_link(const char *path, char mode);

int main(int argc, char *argv[])
{
    /* Basic argument checks */
    if (argc == 1) {
        print_usage(argv[0], stdout);
        return 1;
    }

    if (STREQUALS(argv[1], "--help")) {
        print_usage(argv[0], stderr);
        printf("\nOptions:\n"
               "  -s    Only remove symbolic links.\n"
               "  -l    Only remove hard links.\n"
               "  -v    Be verbose.\n");
        return 0;
    }

    /* Full arguments processing */
    char mode = DELETE_ALL_LINKS;
    bool verbose = false;
    int i;
    for (i = 1; i < argc; i++) {
        if (STREQUALS(argv[i], "--")) {
            i++;
            break;
        } else if (STREQUALS(argv[i], "-v")) {
            verbose = true;
        } else if (STREQUALS(argv[i], "-s")) {
            if (mode != DELETE_ALL_LINKS) {
                fprintf(stderr, "Conflicting options: \"-s\" and \"-l\".\n");
                print_usage(argv[0], stderr);
                return 1;
            }

            mode = DELETE_SYMBOLIC_LINKS;
        } else if (STREQUALS(argv[i], "-l")) {
            if (mode != DELETE_ALL_LINKS) {
                fprintf(stderr, "Conflicting options: \"-s\" and \"-l\".\n");
                print_usage(argv[0], stderr);
                return 1;
            }

            mode = DELETE_HARD_LINKS;
        } else if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                fprintf(stderr, "Invalid flag: %s\n", argv[i]);
                print_usage(argv[0], stderr);
            }

            int j;
            for (j = 1; argv[i][j]; j++) {
                /* Check flags (e.g. -vs instead of -v -s) */
                switch (argv[i][j]) {
                    case 'v':
                        verbose = true;
                        break;
                    case 's':
                        if (mode != DELETE_ALL_LINKS) {
                            fprintf(stderr, "Conflicting options: \"-s\" and \"-l\".\n");
                            print_usage(argv[0], stderr);
                            return 1;
                        }

                        mode = DELETE_SYMBOLIC_LINKS;
                        break;
                    case 'l':
                        if (mode != DELETE_ALL_LINKS) {
                            fprintf(stderr, "Conflicting options: \"-s\" and \"-l\".\n");
                            print_usage(argv[0], stderr);
                            return 1;
                        }

                        mode = DELETE_HARD_LINKS;
                        break;
                    default:
                        printf("Invalid flag: -%c\n", argv[i][j]);
                        print_usage(argv[0], stderr);
                        return 1;
                }
            }
        } else {
            break;
        }
    }

    /* Check if any files were passed */
    if (i == argc) {
        fprintf(stderr, "Missing file operands.\n");
        print_usage(argv[0], stderr);
        return 1;
    }

    /* Remove the files */
    int ret = 0;
    for (; i < argc; i++) {
        ret += remove_link(argv[i], mode, verbose);
    }

    return ret;
}

int remove_link(const char *path, char mode, bool verbose)
{
    struct stat statbuf;
    if (lstat(path, &statbuf)) {
        fprintf(stderr, "Unable to stat \"%s\": %s.\n", path, strerror(errno));
        return 1;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        if (verbose) {
            print_not_a_link(path, mode);
        }

        return 1;
    }

    if (statbuf.st_nlink > 1 && IF_DELETE_HARD_LINKS(mode)) {
        return unlink_path(path, verbose);
    } else if (S_ISLNK(statbuf.st_mode) && IF_DELETE_SYMLINKS(mode)) {
        return unlink_path(path, verbose);
    } else if (verbose) {
        print_not_a_link(path, mode);
    }

    return 1;
}

static int unlink_path(const char *path, bool verbose)
{
    int ret = unlink(path);

    if (ret) {
        if (verbose) {
            fprintf(stderr, "Unable to remove \"%s\": %s.\n", path, strerror(errno));
        }

        return 1;
    }

    if (verbose) {
        printf("Removed \"%s\".\n", path);
    }

    return 0;
}

static void print_usage(const char *program_name, FILE *fh)
{
    fprintf(fh,
           "Usage: %s [-s | -l] [-v] [--] files...\n"
           "       %s --help\n",
           program_name, program_name);
}

static void print_not_a_link(const char *path, char mode)
{
    switch (mode) {
        case DELETE_ALL_LINKS:
            printf("\"%s\" is not a link.\n", path);
            break;
        case DELETE_SYMBOLIC_LINKS:
            printf("\"%s\" is not a symbolic link.\n", path);
            break;
        case DELETE_HARD_LINKS:
            printf("\"%s\" is not a hard link.\n", path);
    }
}

