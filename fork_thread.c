#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* f(void* x){
    printf("Thread: TID=%lu PID=%d PPID=%d\n",
           pthread_self(), getpid(), getppid());
    return 0;
}

int main(){
    pid_t p;

    printf("Parent PID=%d\n", getpid());

    for(int i=0;i<2;i++){
        p = fork();
        if(p==0){
            pthread_t t1,t2;
            printf("Child PID=%d\n", getpid());
            pthread_create(&t1,0,f,0);
            pthread_create(&t2,0,f,0);
            pthread_join(t1,0);
            pthread_join(t2,0);
            return 0;
        }
    }
    return 0;
}
