/**
 * Project spellcorrection
 */


#include "Cache.h"

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sstream>
using std::stringstream;

using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::endl;

/**
 * Cache implementation
 */

/**
 * @param const string &key
 * @param const string & value
 * @return void
 */
void Cache::addElement(const string &key, const string & value) {
    _hashMap.insert(std::make_pair(key,value));
    return;
}

/**
 * @param const string & filename
 * @return void
 */
void Cache::readFromFile(const string & filename) {
    ifstream ifs(filename);
    if(!ifs)
    {
        cout << "open "<<filename<<" error!" << endl;
        exit(-1);
    }
    string query,candidate;
    while(ifs>>query>>candidate)
    {
        _hashMap.insert(std::make_pair(query,candidate));
    }
    ifs.close();
    return;
}

/**
 * @param const string & filename
 * @return void
 */
void Cache::writeToFile(const string & filename) {
    ofstream ofs(filename);
    if(!ofs)
    {
        cout << "open "<<filename<<" error!"<< endl;
        return  ;
    }

    for(auto iter = _hashMap.begin(); iter != _hashMap.end(); ++iter)
    {
        ofs << iter->first << "\t" << iter->second << endl;
    }
    ofs.close();
    return;
}

/**
 * @param const Cache & rhs
 * @return void
 */
void Cache::update(const Cache & rhs) {
    for(auto &pair:rhs._hashMap)
    {
        auto it = _hashMap.find(pair.first);
        if(it == _hashMap.end())
        {
            _hashMap.insert(std::move(pair));
        }
    }
    return;
}

string Cache::query(const string& query)
{
    auto it=_hashMap.find(query);
    if(it!=_hashMap.end())
        return it->second;
    else
        return string();
}
