/**
 * Project spellcorrection
 */


#ifndef _CACHEMANAGER_H
#define _CACHEMANAGER_H

#include <iostream>
#include <string>
#include <vector>


//定时更新每个线程的cache，并且写入磁盘
class Cache;
class TimerThread;
class CacheManager {
public: 
    
    /**
 * @param size_t
 * @param const string &filename
 */

    static void initCache(std::size_t, const std::string &filename);
    
    /**
 * @param size_t idx
 */
    static Cache& getCache(std::size_t idx);
    
    static void periodicUpdateCaches();
private: 
    static  std::vector<Cache> _cacheList;
};

#endif //_CACHEMANAGER_H
