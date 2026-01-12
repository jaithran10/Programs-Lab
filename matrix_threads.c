#include <stdio.h>
#include <pthread.h>

int a[3][3], sum = 0, max, trace = 0;

void* calc_sum(void* p) {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            sum += a[i][j];
    return NULL;
}

void* calc_max(void* p) {
    max = a[0][0];
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(a[i][j] > max)
                max = a[i][j];
    return NULL;
}

void* calc_trace(void* p) {
    for(int i=0;i<3;i++)
        trace += a[i][i];
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    printf("Enter 3x3 matrix:\n");
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            scanf("%d",&a[i][j]);

    pthread_create(&t1,NULL,calc_sum,NULL);
    pthread_create(&t2,NULL,calc_max,NULL);
    pthread_create(&t3,NULL,calc_trace,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);

    printf("Sum = %d\n",sum);
    printf("Max = %d\n",max);
    printf("Trace = %d\n",trace);

    return 0;
}
