#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t child1, child2, grandchild;

    /* Parent creates first child */
    child1 = fork();

    if (child1 == 0) {
        // This is FIRST CHILD
        printf("First Child Process\n");

        /* First child creates its own child (grandchild) */
        grandchild = fork();

        if (grandchild == 0) {
            // This is GRANDCHILD
            printf("Grandchild Process\n");
        } else {
            // First child waits for grandchild
            wait(NULL);
        }
    }
    else {
        // Parent waits for first child to finish
        wait(NULL);

        /* Parent creates second child */
        child2 = fork();

        if (child2 == 0) {
            // This is SECOND CHILD
            printf("Second Child Process\n");
        } else {
            // Parent waits for second child
            wait(NULL);
            printf("Parent Process: All children completed\n");
        }
    }

    return 0;
}
