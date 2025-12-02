#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#define MAX_FILENAME_LENGTH 256
#define SEARCH_PATTERN "content"
#define NUM_THREADS 8
#define SEARCH_DIR "./ToSearch"
#define MAX_LINE_LENGTH 1024

typedef struct Node {
    char filename[MAX_FILENAME_LENGTH];
    pthread_mutex_t file_mutex;
    int thread_number;
    int pattern_found;
    struct Node *next;
} Node;

typedef struct {
    int thread_id;
    FILE *log_file;
} ThreadParam;

extern Node *list_head;
extern pthread_mutex_t list_mutex;


void init(void);
void finish(void);

void GenList(void);
void Add2List(const char *filename);
void ShowList(void);

void *thread_worker(void *arg);

Node *get_next_file(int thread_id);
int search_pattern_in_file(const char *filepath, const char *pattern, FILE *log_file, int thread_id);

#endif /* SEARCH_H */