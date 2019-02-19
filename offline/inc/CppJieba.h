/**
 * Project spellcorrection
 */


#ifndef _CPPJIEBA_H
#define _CPPJIEBA_H

#include "SplitTool.h"
#include "cppjieba/Jieba.hpp"

#include <string>
#include <vector>
#include <map>

class Configuration;
class CppJieba: public SplitTool{
public: 
    CppJieba(std::map<std::string,std::string>& path);
    ~CppJieba(){}
    
    /**
 * @param const string& sentence
 */
    std::vector<std::string> cut(const std::string& sentence);
private:
    cppjieba::Jieba _jieba;
};

#endif //_CPPJIEBA_H
