/**
 * Project spellcorrection
 */


#ifndef _SPELLCORRECTSERVER_H
#define _SPELLCORRECTSERVER_H
#include "TcpConnection.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "TcpConnection.h"
#include "TimerThread.h"
#include "Configuration.h"

#include <string>

class SpellCorrectServer {
public: 
    
    /**
 * @param const string & cfgFileName
 */
    SpellCorrectServer();
    
    void start();
    
    /**
 * @param TcpConnectionPtr conn
 */
    void onConnection(wd::TcpConnectionPtr conn);
    
    /**
 * @param TcpConnectionPtr conn
 */
    void onMessage(wd::TcpConnectionPtr conn);
    
    /**
 * @param TcpConnectionPtr conn
 */
    void onClose(wd::TcpConnectionPtr conn);
private: 
    wd::TcpServer _tcpserver;
    Configuration _conf;
    wd::Threadpool _threadpool;
    wd::TimerThread _timer;
};

#endif //_SPELLCORRECTSERVER_H
