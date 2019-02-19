/**
 * Project spellcorrection
 */


#ifndef _MYTASK_H
#define _MYTASK_H

#include "MyResult.h"
#include "TcpConnection.h"

#include <string>
#include <queue>
#include <vector>
#include <set>


class MyCompare
{
public:
    //priority_queue 默认是operator < ，队头元素最大
    bool operator()(const MyResult & lhs, const MyResult & rhs)
    {
        //距离小优先级高
        if(lhs._iDist > rhs._iDist)
        {
            return true;
        }
        //距离相等，频次多优先级高
        else if(lhs._iDist == rhs._iDist && lhs._iFreq < rhs._iFreq)
        {
            return true;
        }
        //距离相等，频次相等，按字母表顺序排列
        else if(lhs._iDist == rhs._iDist &&
                lhs._iFreq == rhs._iFreq &&
                lhs._word > rhs._word)
        {
            return true;
        }
        else
            return false;
    }
};

class Cache;
class MyTask {
public: 
    
    /**
 * @param const string& queryWord
 * @param int peerfd
 */
    MyTask(const std::string& queryWord, const wd::TcpConnectionPtr& conn);
    
    /**
 * @param Cache& cache
 */
    void execute();
private: 
    std::string _queryWord;
    wd::TcpConnectionPtr _conn;
    std::priority_queue<MyResult, std::vector<MyResult>, MyCompare> _resultQue;
    bool* _bit_array;
    
    void queryIndexTable();
    
    /**
 * @param set& iset
 */
    void statistic(std::set<int>& iset);
    
    /**
 * @param const string& rhs
 */
    int distance(const std::string& rhs);
    
    /**
 * @param Cache& cache
 */
    void response(Cache& cache);

    std::vector<std::string> getChar(const std::string& word);
};

#endif //_MYTASK_H
