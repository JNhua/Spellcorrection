/**
 * Project spellcorrection
 */


#include "Configuration.h"
#include <fstream>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::getline;

bool Configuration::init(const string & filename)
{
    ifstream ifs(filename.c_str());
    if(!ifs)
    {
        cout << "open " <<filename<<" error!"<< endl;
        return false;
    }
    string line;
    while(getline(ifs, line))
    {
        istringstream istr(line);
        string key, value;
        istr >> key >> value;
        _map.insert(make_pair(key, value));
    }
    ifs.close();
    return true;
}

void Configuration::show()
{
    for(auto it=_map.begin(); it != _map.end(); ++it)
    {
        cout << it->first << "\t" << it->second << endl;
    }
}
/**
 * Configuration implementation
 */
