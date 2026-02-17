#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include <math.h>

double mean_value = 0.0;

double median_value = 0.0;

double std_dev_value = 0.0;

int *numbers;

int count;

int compare(const void *a, const void *b) {

 return (*(int*)a - *(int*)b);

}

void *calculate_mean(void *arg) {

 long sum = 0;

 for (int i = 0; i < count; i++) {

 sum += numbers[i];

 }

 mean_value = (double)sum / count;

 printf("Thread 1: Calculated Mean = 

%.2f\n", mean_value);

 pthread_exit(0);

}

void *calculate_median(void *arg) {

 int *sorted = (int *)malloc(count * 

sizeof(int));

 for (int i = 0; i < count; i++) {

 sorted[i] = numbers[i];

 }
qsort(sorted, count, sizeof(int), 

compare);

 if (count % 2 == 0) {

 median_value = (sorted[count/2 - 1] + 

sorted[count/2]) / 2.0;

 } else {

 median_value = sorted[count/2];

 }

 printf("Thread 2: Calculated Median = 

%.2f\n", median_value);

 free(sorted);

 pthread_exit(0);

}

void *calculate_std_dev(void *arg) {

 pthread_t *mean_thread = (pthread_t 

*)arg;

 pthread_join(*mean_thread, NULL);

 double sum_squared_diff = 0.0;

 for (int i = 0; i < count; i++) {

 double diff = numbers[i] -

mean_value;

 sum_squared_diff += diff * diff;

 }

 std_dev_value = sqrt(sum_squared_diff 

/ count);

 printf("Thread 3: Calculated Standard 

Deviation = %.2f\n", std_dev_value);

 pthread_exit(0);

}
int main(int argc, char *argv[]) {

 if (argc < 2) {

 printf("Usage: %s <number1> 

<number2> ... <numberN>\n", argv[0]);

 return 1;

 }

 count = argc - 1;

 numbers = (int *)malloc(count * 

sizeof(int));

 printf("Input numbers: ");

 for (int i = 0; i < count; i++) {

 numbers[i] = atoi(argv[i + 1]);

 printf("%d ", numbers[i]);

 }

 printf("\n\n");

 pthread_t mean_thread, 

median_thread, std_dev_thread;

 printf("Creating worker threads...\n\n");

 pthread_create(&mean_thread, NULL, 

calculate_mean, NULL);

 pthread_create(&median_thread, NULL, 

calculate_median, NULL);

 pthread_create(&std_dev_thread, 

NULL, calculate_std_dev, &mean_thread);

 pthread_join(median_thread, NULL);

 pthread_join(std_dev_thread, NULL); 

 printf(" STATISTICAL ANALYSIS 

RESULTS\n");

 printf("Mean: %.2f\n", 

mean_value);
printf("Median: %.2f\n", 

median_value);

 printf("Standard Deviation: %.2f\n", 

std_dev_value);

 free(numbers);

 return 0;

}
