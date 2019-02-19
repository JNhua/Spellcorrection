/**
 * Project spellcorrection
 */


#ifndef _MYRESULT_H
#define _MYRESULT_H
#include <string>

class MyResult {
    friend class MyCompare;
    friend class MyTask;
public:
    MyResult(std::string word,int iFreq,int iDist)
        :_word(word),_iFreq(iFreq),_iDist(iDist)
    {}
    MyResult(){}

private: 
    std::string _word;
    int _iFreq;
    int _iDist;
};

#endif //_MYRESULT_H
