#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main()
{
    /* Variable declarations */
    struct termios oldterm, newterm;
    int ch;

    /* Initialize raw mode */
    tcgetattr(STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newterm);

    /* Get character */
    printf("Enter a character: ");

    ch = getchar();
    putchar('\n');

    /* Print character */
    if (ch == EOF) {
        printf("<EOF>\n");
    } else {
        printf("Character: '%c'\n", ch);
        printf("Hex: %02x\n", ch);
        printf("Decimal: %d\n", ch);
        printf("Octal: %03o\n", ch);
    }

    /* Deinitialize raw mode */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

    /* Exit */
    return 0;
}

