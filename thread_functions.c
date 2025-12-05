#include "search.h"

void *thread_worker(void *arg) {
    ThreadParam *param = NULL;
    int thread_id = 0;
    FILE *log_file = NULL;
    char log_filename[MAX_FILENAME_LENGTH];
    Node *current_file = NULL;
    char full_path[MAX_FILENAME_LENGTH * 2];
    int files_processed = 0;
    void *return_value = NULL;

    if (arg != NULL) {
        param = (ThreadParam *)arg;
        thread_id = param->thread_id;

        snprintf(log_filename, sizeof(log_filename), "thread_%d.log", thread_id);
        log_file = fopen(log_filename, "w");

        if (log_file != NULL) {
            fprintf(log_file, "=== Thread %d started ===\n", thread_id);
            fflush(log_file);

            current_file = get_next_file(thread_id);

            while (current_file != NULL) {
                snprintf(full_path, sizeof(full_path), "%s/%s",
                        SEARCH_DIR, current_file->filename);

                fprintf(log_file, "[Thread %d] Processing file: %s\n",
                        thread_id, current_file->filename);
                fflush(log_file);

                pthread_mutex_lock(&(current_file->file_mutex));
                current_file->pattern_found = search_pattern_in_file(full_path, SEARCH_PATTERN, log_file, thread_id);
                pthread_mutex_unlock(&(current_file->file_mutex));

                files_processed++;
                current_file = get_next_file(thread_id);
            }

            fprintf(log_file, "=== Thread %d finished ===\n", thread_id);
            fprintf(log_file, "Total files processed: %d\n", files_processed);
            fflush(log_file);

            fclose(log_file);

            printf("Thread %d completed. Processed %d files.\n",
                   thread_id, files_processed);
        } else {
            fprintf(stderr, "Error: Thread %d cannot open log file\n", thread_id);
        }
    }

    return return_value;
}