/*
 * ============================================================================
 * Project:     Multithreaded File Search
 * Institution: HWR Berlin - Hochschule für Wirtschaft und Recht
 * Course:      Operating Systems
 *
 * Authors:     Domenik Wilhelm - Student ID: 77207300494
 *              Theo Leuthardt - Student ID: 77205844868
 *
 * Date:        December 2024
 * Version:     1.0
 * ============================================================================
 */

#include "search.h"

int main(void) {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int i = 0;
    int return_code = 0;
    int thread_created = 0;
    int creation_failed = 0;

    init();
    printf("=== Starting %d threads ===\n", NUM_THREADS);

    for (i = 0; i < NUM_THREADS && creation_failed == 0; i++) {
        thread_ids[i] = i + 1;

        if (pthread_create(&threads[i], NULL, ThrdFunc, &thread_ids[i]) != 0) {
            fprintf(stderr, "Error: Failed to create thread %d\n", i + 1);
            return_code = 1;
            thread_created = i;
            creation_failed = 1;
        } else {
            thread_created = i + 1;
            printf("Thread %d created.\n", i + 1);
        }
    }

    printf("\n=== Waiting for threads to complete ===\n");

    for (i = 0; i < thread_created; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error: Failed to join thread %d\n", i + 1);
            return_code = 1;
        }
    }

    printf("\nAll threads completed.\n");
    finish();

    if (return_code == 0) {
        printf("\n=== Program completed successfully ===\n");
    } else {
        printf("\n=== Program completed with errors ===\n");
    }

    return return_code;
}
