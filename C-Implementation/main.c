#include <stdio.h>
#include <unistd.h>
#include "cache.h"

#define CACHE_SIZE 3

int main() 
{
    LRUCache* cache = create_cache(CACHE_SIZE, 5);

    add_to_cache(cache, "/tmp/test1.txt");
    add_to_cache(cache, "/tmp/test2.txt");

    FileInfo* info = get_entry(cache, "/tmp/test1.txt");
    if (info)
    {
        printf("test1 size: %ld, inode: %lu\n", info->st.st_size, info->st.st_ino);
    }
    else
    {
        printf("test1 not found\n");
    }

    sleep(6); // Sleep past expire time
    add_to_cache(cache, "/tmp/test3.txt");

    info = get_entry(cache, "/tmp/test1.txt");
    if (!info)
    {
        printf("test1 stale and removed\n");
    }
    destroy_cache(cache);
    return 0;
}