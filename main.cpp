#include <iostream>
#include <string>
#include <fstream>

#define CACHE_SZ 2

#include "cache.h"

int main() 
{

    std::ofstream("/tmp/test1.txt");
    std::ofstream("/tmp/test2.txt");
    std::ofstream("/tmp/test3.txt");
    std::ofstream("/tmp/test4.txt");

    cacheManager manager(CACHE_SZ, 60);

    manager.addToCache("/tmp/test1.txt");
    manager.addToCache("/tmp/test2.txt");
    
    FileInfo* info = manager.searchInCache("/tmp/test1.txt");
    if (info) std::cout << "Found: " << info->filePath << std::endl;

    manager.addToCache("/tmp/test4.txt");
    info = manager.searchInCache("/tmp/test1.txt");
    if (!info) std::cout << "Not Found: /tmp/test1.txt" << std::endl;

    manager.pruneCache();

    return 0;
}