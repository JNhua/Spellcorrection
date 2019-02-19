/**
 * Project spellcorrection
 */


#include "CppJieba.h"
#include "Configuration.h"

#include <map>

using std::string;
using std::vector;
using std::map;


CppJieba::CppJieba(map<string,string>& path)
    :_jieba(path["DICT_PATH="],
     path["HMM_PATH="],
     path["USER_DICT_PATH="],
     path["IDF_PATH="],
     path["STOP_WORD_PATH="])
{}


/**
 * @param const string& sentence
 * @return vector<string>
 */
vector<string> CppJieba::cut(const string& sentence) {
    vector<string> words;
    _jieba.Cut(sentence,words,true);
    return words;
}
