#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* Variable declarations */
    long err_no;
    char *invalid;
    int i;

    /* Parse arguments */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [errno]...\n", basename(argv[0]));
        return EXIT_FAILURE;
    }

    for (i = 1; i < argc; i++) {
        invalid = NULL;
        err_no = strtol(argv[i], &invalid, 0);

        if (invalid[0]) {
            printf("Not a number: %s\n", argv[i]);
        } else {
            printf("%s\n", strerror(err_no));
        }
    }

    return EXIT_SUCCESS;
}

