/**
 * Project spellcorrection
 */


#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <string>
#include <map>

class Configuration
{
public:
    std::map<std::string, std::string>& getMap(){	return _map;	}

    bool init(const std::string & filename);

    void show();

private:
    std::map<std::string, std::string> _map;
};

#endif //_CONFIGURATION_H
