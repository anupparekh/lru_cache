#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#pragma once

#define MAX_PATH_LEN 1024

typedef struct FileInfo
{
    char path[MAX_PATH_LEN];
    time_t access_time;
    struct stat st;
    struct FileInfo* next;
    struct FileInfo* prev;
} FileInfo;


typedef struct
{
    int cache_size;
    time_t expire_time;
    int current_size;
    FileInfo* head;
    FileInfo* tail;
} LRUCache;


LRUCache* create_cache(int size, time_t expire_time);
void add_to_cache(LRUCache* cache, const char* path);
FileInfo* get_entry(LRUCache* cache, const char* path);
void remove_from_cache(LRUCache* cache, const char* path);
void remove_stale_entries(LRUCache* cache);
void destroy_cache(LRUCache* cache);



