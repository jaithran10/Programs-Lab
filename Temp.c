#include <stdio.h>
#include <limits.h>

#define MAX 10
#define QUANTUM 3

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
} Process;

/* -------- FCFS -------- */
float fcfs(Process p[], int n) {
    int time = 0, wait = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        wait += time - p[i].arrival;
        time += p[i].burst;
    }
    return (float)wait / n;
}

/* -------- SRTF -------- */
float srtf(Process p[], int n) {
    int remaining[MAX], time = 0, complete = 0;
    int wait[MAX] = {0};

    for (int i = 0; i < n; i++)
        remaining[i] = p[i].burst;

    while (complete < n) {
        int idx = -1, min = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && remaining[i] > 0 && remaining[i] < min) {
                min = remaining[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        remaining[idx]--;

        for (int i = 0; i < n; i++)
            if (i != idx && p[i].arrival <= time && remaining[i] > 0)
                wait[i]++;

        if (remaining[idx] == 0)
            complete++;

        time++;
    }

    int total = 0;
    for (int i = 0; i < n; i++)
        total += wait[i];

    return (float)total / n;
}

/* -------- Priority (Non-Preemptive) -------- */
float priority_np(Process p[], int n) {
    int time = 0, completed = 0;
    int done[MAX] = {0};
    int wait = 0;

    while (completed < n) {
        int idx = -1, high = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && !done[i] && p[i].priority > high) {
                high = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        wait += time - p[idx].arrival;
        time += p[idx].burst;
        done[idx] = 1;
        completed++;
    }
    return (float)wait / n;
}

/* -------- Round Robin -------- */
float round_robin(Process p[], int n) {
    int remaining[MAX], wait[MAX] = {0};
    int time = 0, done;

    for (int i = 0; i < n; i++)
        remaining[i] = p[i].burst;

    do {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && p[i].arrival <= time) {
                done = 0;
                int exec = remaining[i] > QUANTUM ? QUANTUM : remaining[i];

                for (int j = 0; j < n; j++)
                    if (j != i && remaining[j] > 0 && p[j].arrival <= time)
                        wait[j] += exec;

                time += exec;
                remaining[i] -= exec;
            }
        }
    } while (!done);

    int total = 0;
    for (int i = 0; i < n; i++)
        total += wait[i];

    return (float)total / n;
}

/* -------- Main -------- */
int main() {
    int n;
    Process p[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        p[i].pid = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
        printf("Priority: ");
        scanf("%d", &p[i].priority);
    }

    float w1 = fcfs(p, n);
    float w2 = srtf(p, n);
    float w3 = priority_np(p, n);
    float w4 = round_robin(p, n);

    printf("\nAverage Waiting Times:\n");
    printf("FCFS       : %.2f\n", w1);
    printf("SRTF       : %.2f\n", w2);
    printf("Priority   : %.2f\n", w3);
    printf("Round Robin: %.2f\n", w4);

    float min = w1;
    char algo[] = "FCFS";

    if (w2 < min) { min = w2; sprintf(algo, "SRTF"); }
    if (w3 < min) { min = w3; sprintf(algo, "Priority"); }
    if (w4 < min) { min = w4; sprintf(algo, "Round Robin"); }

    printf("\nMinimum Average Waiting Time: %s (%.2f)\n", algo, min);

    return 0;
}
