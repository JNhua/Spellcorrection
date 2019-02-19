#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class Index{
public:
    void init(const std::string& dictEnPath, const std::string& dictCnPath);
    void read_Dict(const std::string& path);
    void create_index_table(int index);
    void store_index_table(const std::string& index_Path);
private:
    std::vector<std::pair<std::string,int>> _dict;
    std::unordered_map<std::string,std::set<int>> _index_table; //索引的hashtable
};

#endif // INDEX_H
