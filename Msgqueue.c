#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX_TEXT 100

struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main() {
    key_t key;
    int msgid;
    pid_t pid;
    struct message msg;
    char input_string[MAX_TEXT];

    key = ftok(".", 'B');
    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    // Child Process
    else if (pid == 0) {

        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        printf("Child: Received string: %s\n", msg.msg_text);

        reverse_string(msg.msg_text);

        printf("Child: Reversed string: %s\n", msg.msg_text);

        msg.msg_type = 2;

        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        exit(0);
    }

    // Parent Process
    else {
        printf("Enter a string: ");
        fgets(input_string, MAX_TEXT, stdin);

        input_string[strcspn(input_string, "\n")] = '\0';

        msg.msg_type = 1;
        strcpy(msg.msg_text, input_string);

        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd failed");
            return 1;
        }

        printf("Parent: Sent string: %s\n", input_string);

        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 2, 0) == -1) {
            perror("msgrcv failed");
            return 1;
        }

        printf("Parent: Received reversed string: %s\n", msg.msg_text);

        wait(NULL);

        if (strcmp(input_string, msg.msg_text) == 0) {
            printf("\nResult: The string IS a palindrome!\n");
        } else {
            printf("\nResult: The string is NOT a palindrome.\n");
        }

        msgctl(msgid, IPC_RMID, NULL);
    }

    return 0;
}
