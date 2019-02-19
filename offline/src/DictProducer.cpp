/**
 * Project spellcorrection
 */


#include "DictProducer.h"
#include "CppJieba.h"

#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <iterator>
#include <ctype.h>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::getline;
using std::get;
using std::vector;
using std::transform;

/**
 * DictProducer implementation
 */


/**
 * @param const string & dir
 */
DictProducer::DictProducer(const string & dir)
    :_dir(dir)
{
    get_files();
    bulid_dict();
}

/**
 * @param const string& dir
 * @param SplitTool   * splitTool
 */
DictProducer::DictProducer(const string& dir, SplitTool* splitTool)
    :_dir(dir),_splitTool(splitTool)
{
    get_files();
    bulid_cn_dict();
}

/**
 * @return void
 */
void DictProducer::bulid_dict() {
    for(auto filename:_files)
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cout << "ifstream open error!" << endl;
            exit(-1);
        }
        //按行读取文本
        string line;
        string word;
        while (getline(ifs, line))	//因为输入流是ifs,所有会遇到eof()自动终止，如果是cin则跳不出while
        {
            //分解出单词
            for (size_t i = 0; i<line.length(); ++i)
            {
                if (!isalpha(line[i]))
                {
                    continue;
                }
                while (i<line.length() && isalpha(line[i]))
                {
                    word += (line[i]>='A'&& line[i]<='Z') ?(line[i]+32):line[i];
                    ++i;
                }
                if(line[i]=='-')
                {
                    char tmp;
                    if(i==line.length()-1)
                    {
                        while(ifs>>tmp)
                        {
                            if(isalpha(tmp))
                                word+=tmp;
                            else
                                break;
                        }
                        ++i;
                    }
                    else
                    {
                        if(isalpha(line[i+1]))
                        {
                            word+=line[i++];
                            while (i<line.length() && isalpha(line[i]))
                            {
                                word += (line[i]>='A'&& line[i]<='Z') ?(line[i]+32):line[i];
                                ++i;
                            }
                        }
                    }
                }
                push_dict(word);
                word.clear();
            }
        }
        ifs.close();
    }
    return;
}

/**
 * @return void
 */
void DictProducer::bulid_cn_dict() {
    for(auto filename:_files)
    {
        ifstream ifs(filename);
        if (!ifs)
        {
            cout << "ifstream open error!" << endl;
            exit(-1);
        }
        //按行读取文本
        string line;
        string lines;
        vector<string> words;
        while (getline(ifs, line))
        {
            //存储整篇文章
            lines.append(line); //这样读入没有'\n'
        }
        //分解单词
        words=_splitTool->cut(lines);
        for(auto word:words)
        {
            transform(word.begin(),word.end(),word.begin(),::tolower);
            push_dict(word);
        }
        ifs.close();
    }
    return;
}

/**
 * @param string & filename
 * @return void
 */
void DictProducer::store_dict(string & filename) {
    ofstream ofs(filename);
    if(!ofs)
    {
        cout<<"input "<<filename<<" error!"<<endl;
        exit(-1) ;
    }
    for(auto pairs:_dict)
    {
        ofs<<pairs.first<<"\t"<<pairs.second<<endl;
    }
    ofs.close();
    return;
}

/**
 * @return void
 */
void DictProducer::show_files() {
    for(auto file:_files)
    {
        cout<<file<<endl;
    }
    return;
}

/**
 * @return void
 */
void DictProducer::show_dict() {
    for(auto it=_dict.begin();it!=_dict.end();++it)
    {
        cout<<it->first<<"\t"<<it->second<<endl;
    }
    return;
}

/**
 * @return void
 */
void DictProducer::get_files() {
    DIR *Dir;
    struct dirent *ptr;
    Dir = opendir(_dir.c_str()); ///open the dir
    string path(_dir+"/");

    while((ptr = readdir(Dir)) != NULL) ///read the list of this dir
    {
        if(!strcmp(ptr->d_name,".")||!strcmp(ptr->d_name,".."))
            continue;
        _files.push_back(path+ptr->d_name);
    }
    closedir(Dir);
    return;
}

/**
 * @param const string& word
 * @return void
 */
void DictProducer::push_dict(const string& word) {
    _dict[word]++;
    return;
}
