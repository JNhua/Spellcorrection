///
/// @file    main.cpp
/// @author  JNhua(hjntc59@foxmail.com)
/// @date    2018-08-23 16:51:15
///

#include "Configuration.h"
#include "SpellCorrectServer.h"
#include "CacheManager.h"
#include "Singleton.h"
#include "Mydict.h"

using std::stoi;

int main(void)
{
    Singleton<Configuration>::getInstance()->init("../conf/config");

    Singleton<Mydict>::getInstance()->init(
                Singleton<Configuration>::getInstance()->getMap()["dict"],
            Singleton<Configuration>::getInstance()->getMap()["cn_dict"],
            Singleton<Configuration>::getInstance()->getMap()["index"]);

    CacheManager::initCache(stoi(Singleton<Configuration>::getInstance()->getMap()["thread_num"]),
            Singleton<Configuration>::getInstance()->getMap()["cache"]);
    //先让CacheManager初始化，获取到cache内容
    //计时器设置在SpellCorrectServer中
    SpellCorrectServer server;
	server.start();

	return 0;
}
