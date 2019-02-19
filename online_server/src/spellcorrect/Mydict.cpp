/**
 * Project spellcorrection
 */


#include "Mydict.h"

#include <fstream>
using std::ifstream;

#include <iostream>
using std::cout;
using std::endl;

#include <sstream>
using std::stringstream;

using std::string;
using std::unordered_map;
using std::vector;
using std::set;
using std::pair;
using std::getline;

/**
 * Mydict implementation
 */


/**
 * @param const char * dictEnPath
 * @param const char * dictCnPath
 * @return void
 */

void Mydict::read_Dict(const string& dictpath)
{
    ifstream ifs(dictpath);
        if(!ifs)
        {
            cout << "open "<<dictpath<<" error!" << endl;
            exit(-1);
        }

        string line;
        while(getline(ifs, line))
        {
            stringstream ss(line);
            string key;
            int value;
            ss >> key >> value;
            _dict.push_back(std::make_pair(key, value));
        }
        ifs.close();
}

void Mydict::init(const std::string& dictEnPath, const std::string& dictCnPath,
                  const std::string& indexPath) {
    read_Dict(dictEnPath);
    read_Dict(dictCnPath);
    //read_Index
    ifstream ifs(indexPath);
    if(!ifs)
    {
        cout << "open "<<indexPath<<" error!" << endl;
        exit(-1);
    }

    string line;
    while(getline(ifs, line))
    {
        stringstream ss(line);
        string key;
        ss>>key;
        int index;
        while(ss>>index)
        {
            _index_table[key].insert(index);
        }
    }
    ifs.close();
    return;
}

/**
 * @return vector<pair<string, int> > &
 */
vector<pair<string, int> > & Mydict::getDict() {
    return _dict;
}

/**
 * @return map<string, set<int> > &
 */
unordered_map<string, set<int> > & Mydict::getIndexTable() {
    return _index_table;
}
