/**
 * Project spellcorrection
 */


#ifndef _CACHE_H
#define _CACHE_H

#include <vector>
#include <unordered_map>
#include <set>
#include <string>


class Cache {
public: 
    
    /**
 * @param const Cache& cache
 */
    
    /**
 * @param const string &key
 * @param const string & value
 */
    void addElement(const std::string &key, const std::string & value);
    
    /**
 * @param const string & filename
 */
    void readFromFile(const std::string & filename);
    
    /**
 * @param const string & filename
 */
    void writeToFile(const std::string & filename);
    
    /**
 * @param const Cache & rhs
 */
    void update(const Cache & rhs);

    std::string query(const std::string& query);
private: 
    std::unordered_map<std::string, std::string> _hashMap;      //缓存的hashmap
};

#endif //_CACHE_H
