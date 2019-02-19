/**
 * Project spellcorrection
 */


#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H

#include <string>
#include <map>
#include <vector>

class SplitTool;
class DictProducer {
public: 
    
    /**
 * @param const string & dir
 */
    DictProducer(const std::string & dir);
    
    /**
 * @param const string& dir
 * @param SplitTool   * splitTool
 */
    DictProducer(const std::string& dir, SplitTool* splitTool);
    
    void bulid_dict();
    
    void bulid_cn_dict();
    
    /**
 * @param string & filename
 */
    void store_dict(std::string & filename);
    
    void show_files();
    
    void show_dict();
private: 
    std::string _dir;   //语料库目录
    std::vector<std::string> _files;    //语料文件的绝对路径集合
    std::map<std::string,int> _dict;    //词典
    SplitTool* _splitTool;
    
    void get_files();
    
    /**
 * @param const string& word
 */
    void push_dict(const std::string& word);
};

#endif //_DICTPRODUCER_H
