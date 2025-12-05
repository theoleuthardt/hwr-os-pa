#include "search.h"

void *ThrdFunc(void *arg) {
    int thread_id = 0;
    FILE *log_file = NULL;
    char log_filename[MAX_FILENAME_LENGTH];
    Node *current = NULL;
    int mutex_locked = 0;
    int search_result = 0;
    void *return_value = NULL;

    if (arg != NULL) {
        thread_id = *((int *)arg);

        snprintf(log_filename, sizeof(log_filename), "thread_%d.log", thread_id);
        log_file = fopen(log_filename, "w");

        if (log_file != NULL) {
            fprintf(log_file, "=== Thread %d started ===\n", thread_id);
            fflush(log_file);

            current = list_head;

            while (current != NULL) {
                if (current->thread_number == 0) {
                    mutex_locked = 0;
                    if (pthread_mutex_trylock(&(current->file_mutex)) == 0) {
                        mutex_locked = 1;
                    }

                    if (mutex_locked == 1) {
                        current->thread_number = thread_id;
                        pthread_mutex_unlock(&(current->file_mutex));

                        fprintf(log_file, "[Thread %d] Processing file: %s\n",
                                thread_id, current->filename);
                        fflush(log_file);

                        search_result = Search(current->filename);

                        pthread_mutex_lock(&(current->file_mutex));
                        current->pattern_found = search_result;
                        pthread_mutex_unlock(&(current->file_mutex));

                        if (search_result == 1) {
                            fprintf(log_file, "[Thread %d] Pattern found in file: %s\n",
                                    thread_id, current->filename);
                        } else {
                            fprintf(log_file, "[Thread %d] Pattern not found in file: %s\n",
                                    thread_id, current->filename);
                        }
                        fflush(log_file);
                    } else {
                        fprintf(log_file, "[Thread %d] File already locked: %s\n",
                                thread_id, current->filename);
                        fflush(log_file);
                    }
                } else {
                    fprintf(log_file, "[Thread %d] File being processed by thread %d: %s\n",
                            thread_id, current->thread_number, current->filename);
                    fflush(log_file);
                }

                current = current->next;
            }

            fprintf(log_file, "=== Thread %d finished ===\n", thread_id);
            fflush(log_file);

            fclose(log_file);
        }
    }

    return return_value;
}