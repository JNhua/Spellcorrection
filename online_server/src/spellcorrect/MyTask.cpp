/**
 * Project spellcorrection
 */


#include "MyTask.h"
#include "CacheManager.h"
#include "Cache.h"
#include "Singleton.h"
#include "Mydict.h"
#include "Thread.h"

#include "json/json.h"

#include <string.h>
using std::set;
using std::string;
using std::vector;
using std::sprintf;

#include <unordered_map>
using std::unordered_map;

#include <iostream>
using std::cout;
using std::endl;

/**
 * MyTask implementation
 */


/**
 * @param const string& queryWord
 * @param int peerfd
 */
MyTask::MyTask(const string& queryWord, const wd::TcpConnectionPtr& conn)
    :_queryWord(queryWord),_conn(conn)
{
    auto dict=Singleton<Mydict>::getInstance()->getDict();
    int bit_len=dict.size();
    _bit_array=new bool[bit_len];
    memset(_bit_array,0,bit_len);
}

/**
 * @param Cache& cache
 * @return void
 */
//查询
void MyTask::execute() {
    //cache查找
    //获取当前线程对应的cache
    Cache& cache=CacheManager::getCache(wd::thread_index);
    string ret=cache.query(_queryWord);
    if(ret!=string())
    {
        //找到
        _conn->sendInLoop(ret);
        cout<<"cache query success!"<<endl;
        return ;
    }

    //index查找
    queryIndexTable();
    //反馈给客户端
    response(cache);

    return;
}

vector<string> MyTask::getChar(const string& word)
{
    size_t it = 0;
    vector<string> ret;
    while(it < word.length())
    {
        if(word[it] & 0x80){
            int i;
            for(i=6;i>=2;--i)
            {
                if(word[it] & (1<<i))
                    continue;
                else
                    break;
            }
            ret.push_back(word.substr(it,8-i));
            it+=8-i;
        }
        else{
            ret.push_back(word.substr(it,1));
            ++it;
        }
    }
    return ret;
}

/**
 * @return void
 */
//在索引表里查询
void MyTask::queryIndexTable() {
    unordered_map<string, set<int> > index_table=
            Singleton<Mydict>::getInstance()->getIndexTable();
    //得到一个字符作为key
    vector<string> char_list=getChar(_queryWord);
    for(auto ch:char_list)
    {
        //索引表中是否存在
        if(index_table.count(ch))
        {
            statistic(index_table[ch]);
        }
    }
    return;
}

/**
 * @param set& iset
 * @return void
 */
//计算
void MyTask::statistic(set<int>& iset) {
    auto dict=Singleton<Mydict>::getInstance()->getDict();
    for(auto idx:iset)
    {
        string candidata=dict[idx].first;
        //去除冗余候选词
        if(_bit_array[idx])
            continue;
        else
            _bit_array[idx]=true;
        int iDist=distance(candidata);
        if(iDist<3)
        {
            _resultQue.push(MyResult(candidata,dict[idx].second,iDist));
        }
    }
    return;
}

/**
 * @param const string& rhs
 * @return int
 */
//计算最小编辑距离
int MyTask::distance(const string& rhs) {
    vector<string> querry=getChar(_queryWord);
    vector<string> candidate = getChar(rhs);
    int len1,len2;
    len1 = querry.size();
    len2 = candidate.size();
    int dis[len1+1][len2+1];
    memset(dis,0,(len1+1)*(len2+1));
    for(int i=0;i<=len1;++i)
        dis[i][0] = i;
    for(int j=0;j<=len2;++j)
        dis[0][j] = j;
    for(int i=1;i<=len1;++i)
    {
        for(int j=1;j<=len2;++j)
        {
            int cost = ((querry[i-1]== candidate[j-1])?0:1);
            int deletion = dis[i-1][j] +1;
            int insertion = dis[i][j-1] + 1;
            int substitution = dis[i-1][j-1] + cost;
            dis[i][j] =  std::min(deletion,std::min(insertion,substitution));
        }
    }
    return dis[len1][len2];
}

/**
 * @param Cache& cache
 * @return void
 */
//响应客户端的请求
void MyTask::response(Cache& cache) {
    if(_resultQue.empty())
    {
        string result = " ";
        _conn->sendInLoop(result);
    }
    else
    {
        //最多返回3个候选词
        Json::FastWriter writer;
        Json::Value array;
        for(int i=0;i<3&&!_resultQue.empty();++i)
        {
            string candidate=_resultQue.top()._word;
            Json::Value new_item;
            char tmp[20]={0};
            sprintf(tmp,"%s%d","candidate ",i);
            new_item[tmp] = candidate;
            _resultQue.pop();
            array.append(new_item);
        }
        string jsonstr=writer.write(array);
        _conn->sendInLoop(jsonstr);
        cache.addElement(_queryWord, jsonstr);
        cout<<"cache add"<<endl;
    }
    cout << "reponse" << endl;
    return;
}
