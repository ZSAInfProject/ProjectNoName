#include "ChunkDatabase.h"
#include "../../utils/Log.h"
#include "../../utils/Settings.h"
#include "../../Game.h"

ChunkDatabase::ChunkDatabase(std::unique_ptr<ChunkGenerator> _chunkGenerator) {
    chunkGenerator = std::move(_chunkGenerator);
}

Chunk *ChunkDatabase::getChunk(int x, int y) {
    //Try to load chunk from cache
    auto chunkIt = chunkCache.find(std::make_tuple(x, y));
    if(chunkIt != chunkCache.end()){
        chunkIt->second.lastHit = std::chrono::system_clock::now();
    }
    else {
        Log::verbose(TAG, "Cache miss X = " + std::to_string(x) + " Y = " + std::to_string(y));
        //If chunk is not in cache load it from file
        std::unique_ptr<Chunk> fileChunk = std::make_unique<Chunk>();
        bool fileExists = fileChunk->load(getChunkFilename(x,y));
        if(fileExists){
            chunkIt = insertChunkIntoCache(x, y, std::move(fileChunk));
        }
        else {
            //if chunk does not exist generate one
            chunkIt = insertChunkIntoCache(x, y, chunkGenerator->generateChunk(x, y));
        }
    }

    cleanUpCache();
    Game::get().debug.reportLoadedChunks(static_cast<int>(chunkCache.size()));
    saveCache();

    return chunkIt->second.chunk.get();
}
std::map<std::tuple<int, int>, CacheEntry>::iterator ChunkDatabase::insertChunkIntoCache(int x, int y, std::unique_ptr<Chunk> chunk) {
    return chunkCache.insert(std::pair<std::tuple<int, int>, CacheEntry>(
            std::make_tuple(x, y), CacheEntry{std::move(chunk), std::chrono::system_clock::now()})).first;
}

void ChunkDatabase::cleanUpCache() {
    auto now = std::chrono::system_clock::now();
    auto sinceLastCleanup = std::chrono::duration_cast<std::chrono::microseconds>(now - lastCacheCleanUp);
    if(sinceLastCleanup > cleanUpPeriod){
        lastCacheCleanUp = std::chrono::system_clock::now();
        if(chunkCache.size() > maxNumOfChunksBeforeCleanup) {
            std::map<std::chrono::system_clock::time_point,
                    std::map<std::tuple<int, int>, CacheEntry>::iterator> cacheEntries;
            int chunksToRelease = (int)chunkCache.size() - maxNumOfChunksBeforeCleanup;
            int releasedChunks = 0;

            for(auto it = chunkCache.begin(); it != chunkCache.end(); it++){
                cacheEntries[it->second.lastHit] = it;
            }
            for(auto cacheEntry : cacheEntries){
                auto sinceLastHit = std::chrono::duration_cast<std::chrono::microseconds>(now - cacheEntry.first);
                if(sinceLastHit < minimumCacheTime || releasedChunks >= chunksToRelease) {
                    Log::verbose(TAG, "Cache cleanup - removed " + std::to_string(releasedChunks) + " entries");
                    return;
                }
                cacheEntry.second->second.chunk->save(
                        getChunkFilename(std::get<0>(cacheEntry.second->first), std::get<1>(cacheEntry.second->first)));
                chunkCache.erase(cacheEntry.second);
                releasedChunks++;
            }
        }
    }
}

std::string ChunkDatabase::cacheDebug(int x, int y) {
    std::vector<std::vector<bool>> vector(80, std::vector<bool>(80, false));

    for (auto const& chunk :chunkCache){
        if (x<std::get<0>(chunk.first)<x+80 and y<std::get<1>(chunk.first)<y+80) {
            vector[std::get<0>(chunk.first)][std::get<1>(chunk.first)] = true;
        }
    }

    std::string string;

    for (int i=0; i<80; i++) {
        for (int j=0; j<80; j++) {
            if (vector[j][i]) string += "X";
            else string += " ";
        }
        string += "\n";
    }

    return string;
}
    
std::string ChunkDatabase::getChunkFilename(int x, int y) {
    std::string filename = Settings::get<std::string>("save_path")+"chunks/";
    if(x < 0){
        filename += "1";
    }else{
        filename += "0";
    }
    if(y < 0){
        filename += "1";
    }else{
        filename += "0";
    }

    filename += "_";
    filename += std::to_string(std::abs(x));
    filename += "_";
    filename += std::to_string(std::abs(y));

    return filename;
}

ChunkDatabase::~ChunkDatabase() {
    saveCache(true);
}

void ChunkDatabase::saveCache(bool ignoreAutoSavePeriod) {
    auto now = std::chrono::system_clock::now();
    auto sinceLastAutoSave = std::chrono::duration_cast<std::chrono::seconds>(now - lastAutoSave);
    if(sinceLastAutoSave > autoSavePeriod || ignoreAutoSavePeriod) {
        lastAutoSave = now;
        for (auto &entry : chunkCache) {
            entry.second.chunk->save(
                    getChunkFilename(std::get<0>(entry.first), std::get<1>(entry.first))
            );
        }
        Log::verbose(TAG, "Cache saved");
    }
}
