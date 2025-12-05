#include "search.h"

Node *list_head = NULL;
pthread_mutex_t list_mutex;

void init(void) {
    list_head = NULL;
    pthread_mutex_init(&list_mutex, NULL);

    printf("=== Initializing search program ===\n");
    printf("Search pattern: %s\n", SEARCH_PATTERN);
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Search directory: %s\n\n", SEARCH_DIR);

    GenList();
}

void finish(void) {
    Node *current = NULL;
    Node *next_node = NULL;

    printf("\n=== Search Results ===\n");
    ShowList();

    printf("\n=== Cleaning up resources ===\n");

    current = list_head;
    while (current != NULL) {
        next_node = current->next;
        pthread_mutex_destroy(&(current->file_mutex));
        free(current);
        current = next_node;
    }

    pthread_mutex_destroy(&list_mutex);

    printf("Cleanup completed.\n");
}

void GenList(void) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    int file_count = 0;
    int is_valid_file = 0;

    dir = opendir(SEARCH_DIR);

    if (dir != NULL) {
        printf("Reading files from directory '%s'...\n", SEARCH_DIR);

        while ((entry = readdir(dir)) != NULL) {
            is_valid_file = 1;

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                is_valid_file = 0;
            }

            if (is_valid_file == 1) {
                if (entry->d_name[0] == '.') {
                    is_valid_file = 0;
                }
            }

            if (is_valid_file == 1) {
                Add2List(entry->d_name);
                file_count++;
            }
        }

        closedir(dir);

        printf("Added %d files to the search list.\n\n", file_count);
    } else {
        fprintf(stderr, "Error: Cannot open directory '%s': %s\n",
                SEARCH_DIR, strerror(errno));
    }
}

void Add2List(const char *filename) {
    Node *new_node = NULL;
    Node *current = NULL;

    if (filename != NULL) {
        new_node = (Node *)malloc(sizeof(Node));

        if (new_node != NULL) {
            strncpy(new_node->filename, filename, MAX_FILENAME_LENGTH - 1);
            new_node->filename[MAX_FILENAME_LENGTH - 1] = '\0';
            pthread_mutex_init(&(new_node->file_mutex), NULL);
            new_node->thread_number = 0;
            new_node->pattern_found = 0;
            new_node->next = NULL;

            pthread_mutex_lock(&list_mutex);

            if (list_head == NULL) {
                list_head = new_node;
            } else {
                current = list_head;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_node;
            }

            pthread_mutex_unlock(&list_mutex);
        } else {
            fprintf(stderr, "Error: Memory allocation failed for node\n");
        }
    }
}

void ShowList(void) {
    Node *current = NULL;
    int total_files = 0;
    int files_with_pattern = 0;

    current = list_head;

    printf("%-30s %-10s %-15s\n", "Filename", "Thread", "Pattern Found");
    printf("-----------------------------------------------------------\n");

    while (current != NULL) {
        printf("%-30s %-10d %-15s\n",
               current->filename,
               current->thread_number,
               current->pattern_found ? "YES" : "NO");

        total_files++;
        if (current->pattern_found) {
            files_with_pattern++;
        }

        current = current->next;
    }

    printf("-----------------------------------------------------------\n");
    printf("Total files: %d | Files with pattern: %d\n",
           total_files, files_with_pattern);
}

Node *get_next_file(int thread_id) {
    Node *current = NULL;
    Node *result = NULL;
    int found = 0;

    pthread_mutex_lock(&list_mutex);

    current = list_head;
    while (current != NULL && found == 0) {
        if (current->thread_number == 0) {
            if (pthread_mutex_trylock(&(current->file_mutex)) == 0) {
                current->thread_number = thread_id;
                result = current;
                pthread_mutex_unlock(&(current->file_mutex));
                found = 1;
            }
        }

        if (found == 0) {
            current = current->next;
        }
    }

    pthread_mutex_unlock(&list_mutex);
    return result;
}

int search_pattern_in_file(const char *filepath, const char *pattern,
                          FILE *log_file, int thread_id) {
    FILE *file = NULL;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int found = 0;
    int result = 0;

    file = fopen(filepath, "r");

    if (file != NULL) {
        fprintf(log_file, "[Thread %d] Searching in file: %s\n",
                thread_id, filepath);
        fflush(log_file);

        while (fgets(line, sizeof(line), file) != NULL) {
            line_number++;
            if (strstr(line, pattern) != NULL) {
                if (found == 0) {
                    result = 1;
                    found = 1;
                }
                fprintf(log_file, "[Thread %d] Pattern found in %s at line %d\n",
                        thread_id, filepath, line_number);
                fflush(log_file);
            }
        }

        if (found == 0) {
            fprintf(log_file, "[Thread %d] Pattern not found in %s\n",
                    thread_id, filepath);
            fflush(log_file);
        }

        fclose(file);
    } else {
        fprintf(log_file, "[Thread %d] Error opening file %s: %s\n",
                thread_id, filepath, strerror(errno));
        fflush(log_file);
    }

    return result;
}