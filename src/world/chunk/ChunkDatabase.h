#ifndef NONAME_CHUNKDATABASE_H
#define NONAME_CHUNKDATABASE_H


#include <map>
#include "Chunk.h"
#include "ChunkGenerator.h"

struct CacheEntry {
    static constexpr auto TAG = "CacheEntry";

    std::unique_ptr<Chunk> chunk;
    std::chrono::system_clock::time_point lastHit;
};

//! Class that manages Chunk storage, loading and generation
/*!
 * ChunkDatabase stores recently used chunks in cache and loads or generates new chunks
 * if they are not in the cache.
 */
class ChunkDatabase {

public:
    static constexpr auto TAG = "ChunkDatabase";

    std::map<std::tuple<int, int>, CacheEntry> chunkCache;

    //! Gets Chunk from database
    /*!
     * Return a pointer to requested Chunk. Any changes to the Chunk are
     * going to get saved automatically.
     * @param x X coordinate of Chunk
     * @param y Y coordinate of Chunk
     * @return pointer to requested Chunk
     */
    Chunk* getChunk(int x, int y);

    //! Method for debugging cache
    /*!
     * Returns string representation of cache (what chunks are loaded).
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-right corner
     * @return string representation of cache
     */

    explicit ChunkDatabase(std::unique_ptr<ChunkGenerator>);
    ~ChunkDatabase();
private:
    std::unique_ptr<ChunkGenerator> chunkGenerator;

    std::map<std::tuple<int, int>, CacheEntry>::iterator
    insertChunkIntoCache(int x, int y, std::unique_ptr<Chunk> chunk);

    std::chrono::system_clock::time_point lastCacheCleanUp = std::chrono::system_clock::now();
    std::chrono::microseconds cleanUpPeriod = std::chrono::microseconds(50000);
    std::chrono::microseconds minimumCacheTime = std::chrono::microseconds(100000);
    const int maxNumOfChunksBeforeCleanup = 10;
    void cleanUpCache();

    std::chrono::system_clock::time_point lastAutoSave = std::chrono::system_clock::now();
    std::chrono::seconds autoSavePeriod = std::chrono::seconds(10);
    void saveCache(bool ignoreAutoSavePeriod = false);

    std::string getChunkFilename(int x, int y);
};


#endif //NONAME_CHUNKDATABASE_H
