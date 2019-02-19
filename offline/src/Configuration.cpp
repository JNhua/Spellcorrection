/**
 * Project spellcorrection
 */


#include "Configuration.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

using std::string;
using std::map;
using std::cout;
using std::ifstream;
using std::istringstream;

/**
 * Configuration implementation
 */


/**
 * @param const string& filepath
 */
Configuration::Configuration(const string& filepath)
    :_filepath(filepath)
{
    ifstream ifs(_filepath, std::ios::in);
    if (!ifs)
    {
        sleep(1);
        for (int i = 3; i >= 0; i--)
        {
            cout<<"打开"<<_filepath<<"时出现以下错误：\n";
            perror("fopen");
            printf("\n系统将在%d秒后退出！\n", i);
            sleep(1);
        }
        exit(1);
    }
    string line,key,path;
    while (getline(ifs, line) )
    {
        istringstream istr(line);
        istr>>key;
        istr>>path;
        _configMap.insert(std::make_pair(key,path));
    }
}

/**
 * @return map <string,string>&
 */
map <string,string>& Configuration::getConfigMap() {
    return _configMap;
}
