#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int num1, num2;
    char op;
    pid_t pid;

    printf("Enter num1: ");
    scanf("%d", &num1);

    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &op);

    printf("Enter num2: ");
    scanf("%d", &num2);

    pid = fork();

    if (pid == 0)
    {
        // Child process
        printf("Child Process: Executing calculator...\n");

        char num1_str[10], num2_str[10], op_str[2];

        sprintf(num1_str, "%d", num1);
        sprintf(num2_str, "%d", num2);
        sprintf(op_str, "%c", op);

        char *args[] = {"./calculator", num1_str, op_str, num2_str, NULL};

        execvp(args[0], args);

        // If execvp fails
        perror("execvp failed");
    }
    else
    {
        // Parent process
        wait(NULL);
    }

    return 0;
}