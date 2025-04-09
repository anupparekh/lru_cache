#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

LRUCache* create_cache(int size, time_t expire_time)
{
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    if (!cache) return NULL;

    cache->head = cache->tail = NULL;
    cache->cache_size = size;
    cache->expire_time = expire_time;
    cache->current_size = 0;

    return cache;
}

static void remove_from_tail(LRUCache* cache)
{
    if (!cache->tail) return;

    FileInfo* node = cache->tail;

    if (node->prev) node->prev->next = NULL;
    else cache->head = NULL;

    cache->tail = node->prev;

    free(node);
    cache->current_size--;
}

static void move_to_front(LRUCache* cache, FileInfo* node)
{
    if (cache->head == node) return;

    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (cache->tail == node) cache->tail = node->prev;

    // Move to front
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

void add_to_cache(LRUCache* cache, const char* path)
{
    remove_stale_entries(cache);

    FileInfo* node = cache->head;

    // If path is already there in the cache, then just update the access time
    while (node) {
        if (strcmp(node->path, path) == 0) {
            node->access_time = time(NULL);
            move_to_front(cache, node);
            return;
        }
        node = node->next;
    }

    //Initialize file info 
    FileInfo* new_node = (FileInfo*)malloc(sizeof(FileInfo));
    if (!new_node) return;

    strncpy(new_node->path, path, MAX_PATH_LEN);
    new_node->access_time = time(NULL);
    stat(path, &new_node->st);
    new_node->prev = new_node->next = NULL;

    // Add the new node in cache
    new_node->next = cache->head;
    if (cache->head) cache->head->prev = new_node;
    cache->head = new_node;
    if (!cache->tail) cache->tail = new_node;
    cache->current_size++;

    while (cache->current_size > cache->cache_size)
    {
        remove_from_tail(cache);
    }

}

FileInfo* get_entry(LRUCache* cache, const char* path)
{
    remove_stale_entries(cache);
    FileInfo* node = cache->head;

    while (node)
    {
        if (strcmp(node->path, path) == 0)
        {
            node->access_time = time(NULL);
            move_to_front(cache, node);
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void remove_from_cache(LRUCache* cache, const char* path)
{
    FileInfo* node = cache->head;

    while (node)
    {
        if (strcmp(node->path, path) == 0)
        {
            if (node->prev) node->prev->next = node->next;
            if (node->next) node->next->prev = node->prev;
            if (cache->head == node) cache->head = node->next;
            if (cache->tail == node) cache->tail = node->prev;

            free(node);
            cache->current_size--;
            return;
        }

        node = node->next;
    }

}

void remove_stale_entries(LRUCache* cache)
{
    time_t now = time(NULL);
    FileInfo* node = cache->tail;

    while (node)
    {
        FileInfo* prev = node->prev;
        if (now - node->access_time > cache->expire_time)
        {
            if (node->prev) node->prev->next = node->next;
            if (node->next) node->next->prev = node->prev;
            if (cache->head == node) cache->head = node->next;
            if (cache->tail == node) cache->tail = node->prev;

            free(node);
            cache->current_size--;
        }

        node = prev;
    }
}

void destroy_cache(LRUCache* cache)
{
    FileInfo* node = cache->head;

    while (node)
    {
        FileInfo* temp = node->next;
        free(node);
        node = temp;
    }

    cache->head = cache->tail = NULL;
    cache->current_size = 0;

    free(cache);
}