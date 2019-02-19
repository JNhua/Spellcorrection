/**
 * Project spellcorrection
 */


#ifndef _MYDICT_H
#define _MYDICT_H

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class Mydict {
public: 
    
    /**
 * @param const char * dictEnPath
 * @param const char * dictCnPath
 */
    void read_Dict(const std::string& dictpath);
    void init(const std::string& dictEnPath, const std::string& dictCnPath,
              const std::string& indexPath);
    
    std::vector<std::pair<std::string, int> > & getDict();
    
    std::unordered_map<std::string, std::set<int> > & getIndexTable();
private: 
    std::vector<std::pair<std::string,int>> _dict;
    std::unordered_map<std::string,std::set<int>> _index_table; //索引的hashtable
};

#endif //_MYDICT_H
