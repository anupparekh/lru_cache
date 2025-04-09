#include <cache.h>

lruCache::lruCache(size_t size, time_t pruneTime)
    : cacheSize(size), pruneTime(pruneTime)
{
    std::cout << "Cache size is " << cacheSize << " and prune time of cach entries is " << pruneTime << std::endl;
}

void lruCache::insert(const std::string &filePath)
{
    struct stat st;

    if (stat(filePath.c_str(), &st) != 0)
    {
        std::cout << "Failed to get stat info for file" << filePath;
        return;
    }

    if (fileMap.find(filePath) != fileMap.end()) {
        cacheLruList.erase(fileMap[filePath]);
    } else if (cacheLruList.size() >= cacheSize) {
        fileMap.erase(cacheLruList.back().filePath);
        std::cout << "Removed " << cacheLruList.back().filePath << " from the cache." << std::endl;
        cacheLruList.pop_back();
    }

    cacheLruList.push_front({filePath, std::time(nullptr), st});
    fileMap[filePath] = cacheLruList.begin();

    std::cout << "Added " << filePath << " to the cache." << std::endl;


}

FileInfo* lruCache::find(const std::string &filePath)
{
    if (fileMap.find(filePath) == fileMap.end()) return nullptr;

    cacheLruList.splice(cacheLruList.begin(), cacheLruList, fileMap[filePath]);
    fileMap[filePath] = cacheLruList.begin();
    return &(*cacheLruList.begin());
}

void lruCache::remove(const std::string &filePath)
{
    if (fileMap.find(filePath) == fileMap.end()) return;

    cacheLruList.erase(fileMap[filePath]);
    fileMap.erase(filePath);

    std::cout << "Removed " << filePath << " from the cache." << std::endl;
}


void lruCache::prune()
{
    time_t now = std::time(nullptr);

    while (!cacheLruList.empty() && (now - cacheLruList.back().lastAccessTime) > pruneTime) {
        fileMap.erase(cacheLruList.back().filePath);
        cacheLruList.pop_back();
    }
    std::cout << "Cache Cleared." << std::endl;

}
