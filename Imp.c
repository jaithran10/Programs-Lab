#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <ctype.h>
#define SHM_SIZE 1024
// Function to flip case of string
void flip_case(char *str) {
 for (int i = 0; str[i] != '\0'; i++) {
 if (isupper(str[i])) {
 str[i] = tolower(str[i]);
 } else if (islower(str[i])) {
 str[i] = toupper(str[i]);
 }
 }
}
int main() {
 key_t key;
 int shmid;
 char *shared_memory;
 pid_t pid;
 
 char str1[100], str2[100], str3[100];
 
 // Generate unique key
 key = ftok(".", 'C');
 
 // Create shared memory segment
 shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
 if (shmid == -1) {
 perror("shmget failed");
 return 1;
 }
 
 // Attach shared memory
 shared_memory = (char *)shmat(shmid, NULL, 0);
 if (shared_memory == (char *)-1) {
 perror("shmat failed");
 return 1;
 }
 
 pid = fork();
 
 if (pid < 0) {
 fprintf(stderr, "Fork failed\n");
 return 1;
 }
 else if (pid == 0) {
 // Child process: Read, concatenate, and write back
 
 sleep(1); // Wait for parent to write
 
 // Read three strings from shared memory
 char *ptr = shared_memory;
 strcpy(str1, ptr);
 ptr += strlen(str1) + 1;
 strcpy(str2, ptr);
 ptr += strlen(str2) + 1;
 strcpy(str3, ptr);
 
 printf("Child: Received strings: '%s', '%s', '%s'\n", str1, str2, 
str3);
 
 // Concatenate strings
 char concatenated[SHM_SIZE];
 sprintf(concatenated, "%s %s %s", str1, str2, str3);
 printf("Child: Concatenated string: '%s'\n", concatenated);
 
 // Write back to shared memory
 strcpy(shared_memory, concatenated);
 
 // Detach shared memory
 shmdt(shared_memory);
 exit(0);
 }
 else {
 // Parent process: Write three strings
 
 printf("Enter first string: ");
 fgets(str1, 100, stdin);
 str1[strcspn(str1, "\n")] = 0;
 
 printf("Enter second string: ");
 fgets(str2, 100, stdin);
 str2[strcspn(str2, "\n")] = 0;
 
 printf("Enter third string: ");
 fgets(str3, 100, stdin);
 str3[strcspn(str3, "\n")] = 0;
 
 // Write three strings to shared memory
 char *ptr = shared_memory;
 strcpy(ptr, str1);
 ptr += strlen(str1) + 1;
 strcpy(ptr, str2);
 ptr += strlen(str2) + 1;
 strcpy(ptr, str3);
 
 printf("Parent: Sent three strings to child\n\n");
 
 wait(NULL); // Wait for child to complete
 
 // Read concatenated string
 char result[SHM_SIZE];
 strcpy(result, shared_memory);
 printf("\nParent: Received concatenated string: '%s'\n", result);
 
 // Flip case
 flip_case(result);
 printf("Parent: After flipping case: '%s'\n", result);
 
 // Detach and remove shared memory
 shmdt(shared_memory);
 shmctl(shmid, IPC_RMID, NULL);
 }
 
 return 0;
}