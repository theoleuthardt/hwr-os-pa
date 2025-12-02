#include "search.h"

int main(void) {
    pthread_t threads[NUM_THREADS];
    ThreadParam thread_params[NUM_THREADS];
    int i = 0;
    int return_code = 0;
    int thread_created = 0;
    int creation_failed = 0;

    init();
    printf("=== Starting %d threads ===\n", NUM_THREADS);

    for (i = 0; i < NUM_THREADS && creation_failed == 0; i++) {
        thread_params[i].thread_id = i + 1;
        thread_params[i].log_file = NULL;

        if (pthread_create(&threads[i], NULL, thread_worker,
                          &thread_params[i]) != 0) {
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
