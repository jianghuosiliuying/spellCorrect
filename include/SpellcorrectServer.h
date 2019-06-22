#pragma once

#include "../include/TcpConnection.h"
#include "../include/Threadpool.h"
#include "../include/TcpServer.h"
#include "../include/Configuration.h"

namespace mm
{

class SpellcorrectServer//文本纠错类
{
public:
    SpellcorrectServer(const string & conffileName);
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

	void start();

private:
    Configuration * conf_;//更改为单例模式
	Threadpool threadpool_;
	TcpServer server_;
};

}//end of namespace mm
