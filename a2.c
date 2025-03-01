#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include "a2_helper.h"

sem_t sem; // Semaphore for P3 thread synchronization
sem_t sem_t4_start, sem_t4_end;

void* threadP4Function(void* arg) {
    int thread_no = *(int*)arg;
    if (thread_no == 2) {
        sem_post(&sem_t4_start); // Let T4.1 start after T4.2 has started
    }
    if (thread_no == 1) {
        sem_wait(&sem_t4_start); // Wait for T4.2 to start
    }
    info(BEGIN, 4, thread_no);
    if (thread_no == 1) {
        // Execute task of T4.1 here
        sem_post(&sem_t4_end);   // Signal that T4.1 is ending
    }
    if (thread_no == 2) {
        sem_wait(&sem_t4_end);   // Wait to finish until T4.1 signals it's ending
    }
    info(END, 4, thread_no);
    return NULL;
}

void* threadP3Function(void* arg) {
    int thread_no = *(int*)arg;
    sem_wait(&sem); // Control concurrency
    info(BEGIN, 3, thread_no);
    if (thread_no == 10) {
        // Ensure that T3.10 ends while exactly 5 threads are running
        sem_post(&sem);
        sem_wait(&sem);
    }
    // Simulate work for P3 threads here
    info(END, 3, thread_no);
    sem_post(&sem);
    return NULL;
}

void* threadP7Function(void* arg) {
    int thread_no = *(int*)arg;
    info(BEGIN, 7, thread_no);
    // Simulate work for P7 threads here
    info(END, 7, thread_no);
    return NULL;
}

int main(int argc, char **argv) {
    init(); // Initialize the tester
    info(BEGIN, 1, 0); // Main process starts

    int pid;
    // Creating P2
    if ((pid = fork()) == 0) {
        info(BEGIN, 2, 0); // P2 starts
        info(END, 2, 0);   // P2 ends
        exit(0);
    }
    waitpid(pid, NULL, 0); // Wait for P2 to finish

    // Creating P3
    if ((pid = fork()) == 0) {
        info(BEGIN, 3, 0); // P3 starts
        sem_init(&sem, 0, 5); // Initialize semaphore for 5 concurrent threads

        pthread_t threads[47];
        int thread_ids[47];
        for (int i = 0; i < 47; i++) {
            thread_ids[i] = i + 1;
            pthread_create(&threads[i], NULL, threadP3Function, &thread_ids[i]);
        }
        for (int i = 0; i < 47; i++) {
            pthread_join(threads[i], NULL);
        }

        sem_destroy(&sem);
        info(END, 3, 0); // P3 ends
        exit(0);
    }
    waitpid(pid, NULL, 0); // Wait for P3 to finish

    // Continue creating other processes (P4 to P7)
    if ((pid = fork()) == 0) { // P4
        info(BEGIN, 4, 0);

        sem_init(&sem_t4_start, 0, 0);
        sem_init(&sem_t4_end, 0, 0);
        // Create threads for P4
        pthread_t threads[4];
        int thread_ids[4] = {1, 2, 3, 4};
        for (int i = 0; i < 4; i++) {
            pthread_create(&threads[i], NULL, threadP4Function, &thread_ids[i]);
        }
        for (int i = 0; i < 4; i++) {
            pthread_join(threads[i], NULL);
        }
        sem_destroy(&sem_t4_start);
        sem_destroy(&sem_t4_end);

        info(END, 4, 0);
        exit(0);
    }
    waitpid(pid, NULL, 0); // Wait for P4 to finish

    // Nested creation of P5 to P7 within the respective blocks with similar structure
    if ((pid = fork()) == 0) { // P5
        info(BEGIN, 5, 0);
        info(END, 5, 0);
        exit(0);
    }
    waitpid(pid, NULL, 0); // Wait for P5 to finish

    if ((pid = fork()) == 0) { // P6
        info(BEGIN, 6, 0);
        info(END, 6, 0);
        exit(0);
    }
    waitpid(pid, NULL, 0); // Wait for P6 to finish

    if ((pid = fork()) == 0) { // P7
        info(BEGIN, 7, 0);
        pthread_t threads[6];
        int thread_ids[6] = {1, 2, 3, 4, 5, 6};
        for (int i = 0; i < 6; i++) {
            pthread_create(&threads[i], NULL, threadP7Function, &thread_ids[i]);
        }
        for (int i = 0; i < 6; i++) {
            pthread_join(threads[i], NULL);
        }
        info(END, 7, 0);
        exit(0);
    }
    waitpid(pid, NULL, 0); // Wait for P7 to finish

    info(END, 1, 0); // Main process ends
    return 0;
}