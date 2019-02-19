/**
 * Project spellcorrection
 */


#include "CacheManager.h"
#include "Cache.h"
#include "Singleton.h"
#include "Configuration.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

vector<Cache> CacheManager::_cacheList; //初始化静态变量
/**
 * CacheManager implementation
 */


/**
 * @param size_t
 * @param const string &filename
 * @return void
 */
void CacheManager::initCache(size_t list_num, const string &filename) {
    Cache cache;
    cache.readFromFile(filename);
    for(size_t i=0;i<list_num;++i)
    {
        _cacheList.push_back(cache);
    }
    return;
}

/**
 * @param size_t idx
 * @return Cache&
 */
Cache& CacheManager::getCache(size_t idx) {
    return _cacheList[idx];
}

/**
 * @return void
 */
void CacheManager::periodicUpdateCaches() {
    auto cache0_itr=_cacheList.begin();
    for(auto itr=cache0_itr+1;itr!=_cacheList.end();++itr)
    {
        cache0_itr->update(*itr);
    }
    for(auto itr=cache0_itr+1;itr!=_cacheList.end();++itr)
    {
        itr->update(*cache0_itr);
    }
    cache0_itr->writeToFile(Singleton<Configuration>::getInstance()->getMap()["cache"]);
    cout<<"Cache update!"<<endl;
    return;
}
