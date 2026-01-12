#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int num1, num2;
    char op;

    // Check argument count
    if (argc != 4)
    {
        printf("Usage: %s <num1> <operator> <num2>\n", argv[0]);
        return 1;
    }

    num1 = atoi(argv[1]);
    op = argv[2][0];
    num2 = atoi(argv[3]);

    switch (op)
    {
        case '+':
            printf("Result: %d + %d = %d\n", num1, num2, num1 + num2);
            break;

        case '-':
            printf("Result: %d - %d = %d\n", num1, num2, num1 - num2);
            break;

        case '*':
            printf("Result: %d * %d = %d\n", num1, num2, num1 * num2);
            break;

        case '/':
            if (num2 == 0)
            {
                printf("Error: Division by zero\n");
            }
            else
            {
                printf("Result: %d / %d = %d\n", num1, num2, num1 / num2);
            }
            break;

        default:
            printf("Invalid operator\n");
    }

    return 0;
}