#include "DictProducer.h"
#include "Configuration.h"
#include "CppJieba.h"
#include "index.h"

#include <iostream>
using namespace std;

int main()
{
     string config_file_path;
    cout<<"pls input dict_config_file path: ";
    config_file_path="../config/yuliao";cout<<config_file_path<<endl;
    //cin >> config_file_path;
    map<string,string> data_dir(
               Configuration(config_file_path).getConfigMap());
    string eng_store_path=data_dir["data_dir:"]+"/";

    DictProducer eng_dictproducer(data_dir["eng_dir:"]);
    eng_dictproducer.show_files();
    eng_dictproducer.store_dict(eng_store_path.append("english.dat"));

    cout<<"pls input cppjieba_config_file path: ";
    //cin >> config_file_path;
    config_file_path="../config/cppjieba_config";cout<<config_file_path<<endl;
    CppJieba jieba(Configuration(config_file_path).getConfigMap());
    DictProducer cn_dictproducer(data_dir["cn_dir:"],&jieba);
    string cn_store_path=data_dir["data_dir:"]+"/";
    cn_dictproducer.show_files();
    cn_dictproducer.store_dict(cn_store_path.append("chinese.dat"));

    string index_store_path=data_dir["data_dir:"]+"/";
    Index index_table;
    index_table.init(eng_store_path,cn_store_path);
    index_table.store_index_table(index_store_path.append("index.dat"));

    return 0;
}
