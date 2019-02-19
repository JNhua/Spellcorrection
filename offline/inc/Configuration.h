/**
 * Project spellcorrection
 */


#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <string>
#include <map>

class Configuration {
public: 
    
    /**
 * @param const string& filepath
 */
    Configuration(const std::string& filepath);
    
    std::map <std::string,std::string>& getConfigMap();
private: 
    std::string _filepath;
    std::map<std::string,std::string> _configMap;
};

#endif //_CONFIGURATION_H
