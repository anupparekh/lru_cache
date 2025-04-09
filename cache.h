#include <iostream>
#include <unordered_map>
#include <list>
#include <queue>
#include <sys/stat.h>
#include <ctime>



struct FileInfo
{
    std::string filePath;
    time_t lastAccessTime;
    struct stat st;
};

class lruCache
{
    private:
        size_t cacheSize;
        time_t pruneTime;
        std::list<FileInfo> cacheLruList;
        std::unordered_map<std::string, std::list<FileInfo>::iterator> fileMap;
        
    public:
        lruCache(size_t size, time_t pruneTime);

        void insert(const std::string &filepath);

        FileInfo* find(const std::string &filepath);

        void remove(const std::string &filepath);

        void prune();
};


class cacheManager
{
    private:
        lruCache cache;
    public:

        cacheManager(size_t cacheSize, time_t pruneTime) : 
            cache(cacheSize, pruneTime) {}

        void addToCache(const std::string& filePath) { cache.insert(filePath); }

        FileInfo* searchInCache(const std::string &filepath) { return cache.find(filepath); }

        void removeFromCache(const std::string &filepath) { cache.remove(filepath); }

        void pruneCache()   { cache.prune(); }
};

