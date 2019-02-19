#include "index.h"
#include <fstream>
using std::ifstream;

#include <iostream>
using std::cout;
using std::endl;

#include <sstream>
using std::stringstream;

#include <iterator>

using std::string;
using std::getline;
using std::unordered_map;
using std::set;
using std::vector;

void Index::read_Dict(const string& dictpath)
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

void Index::init(const string& dictPath, const string& cnDictPath)
{
    read_Dict(dictPath);
    read_Dict(cnDictPath);
    //创建索引表
    for(size_t idx = 0; idx != _dict.size(); ++idx)
    {
        create_index_table(idx);
    }
}

void Index::create_index_table(int index)
{
    string key;
    string word=_dict[index].first;
    for(std::size_t idx = 0; idx != word.size(); ++idx)
    {
        char ch = word[idx];
        if(ch & (1 << 7))	//大于128的编码
        {   //utf-8编码的中文
            if((ch & 0xF0) == 0xC0 || (ch & 0xF0) == 0xD0)	//两个字节
            {
                key = word.substr(idx, 2);
                ++idx;	//每次都少+1，是因为for循环会自动加1
            }
            else if((ch & 0xF0) == 0xE0)	//三个字节
            {
                key = word.substr(idx, 3);
                idx += 2;
            }
            //四个字节
            else if((ch & 0xFF) >= 0xF0 && (ch & 0xFF) <= 0xF7)
            {
                key = word.substr(idx, 4);
                idx += 3;
            }
            //5个字节
            else if((ch & 0xFF) >= 0xF8 && (ch & 0xFF) <= 0xFB)
            {
                key = word.substr(idx, 5);
                idx += 4;
            }
            //6个字节
            else if((ch & 0xFF) == 0xFC)
            {
                key = word.substr(idx, 6);
                idx += 5;
            }
        }
        //英文
        else
        {
            key = word.substr(idx, 1);
        }
        _index_table[key].insert(index);
    }
}

void Index::store_index_table(const string& index_Path)
{
    std::ofstream ofs(index_Path);
    if(!ofs)
    {
        cout<<"open "<<index_Path<<" error!"<<endl;
        exit(-1);
    }
    for(auto it=_index_table.begin();it!=_index_table.end();++it)
    {
        ofs<<it->first<<"\t";
        for(auto set_int:it->second)
        {
            ofs<<set_int<<" ";
        }
        ofs<<endl;
    }
    ofs.close();
}
