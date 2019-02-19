/**
 * Project spellcorrection
 */


#ifndef _SPLITTOOL_H
#define _SPLITTOOL_H
#include <vector>
#include <string>

class SplitTool {
public: 
    
    SplitTool(){}
    
/**
 * @param const string& sentence
 */
    virtual std::vector<std::string> cut(const std::string& sentence) = 0;
    virtual ~SplitTool(){}
};

#endif //_SPLITTOOL_H
