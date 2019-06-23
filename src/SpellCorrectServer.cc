#include "../include/SpellcorrectServer.h"
#include "../include/MyTask.h"
#include "../include/Mydict.h"
#include "../include/CacheManger.h"
#include "../include/Configuration.h"
#include <iostream>

using namespace std;

namespace mm
{
SpellcorrectServer::SpellcorrectServer(const string & conffileName)
//SpellcorrectServer::SpellcorrectServer()
:conf_(Configuration::createConfiguration()->initConf(conffileName))//初始化配置信息
//:conf_(Configuration::createConfiguration())//初始化配置信息
,cacheM_(CacheManger::createCacheManger()->init())//初始化主cache,并首次同步cache
,threadpool_(stoi(conf_->getConfigMap().find("threadNum")->second),
             stoi(conf_->getConfigMap().find("queSize")->second))
//,threadpool_(4,10)
,server_(conf_->getConfigMap().find("ip")->second,stoi(conf_->getConfigMap().find("port")->second))
{
    Mydict * pmydict=Mydict::createMydict();//也可以放到Mydict构造函数里
    pmydict->initEn(conf_->getConfigMap().find("dict")->second,
                    conf_->getConfigMap().find("index")->second);//构建英文词典和索引表
    threadpool_.start();
}

//回调函数体现了扩展性
void SpellcorrectServer::onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << " has connected!" << endl;
	conn->send("welcome to server.");
}

void SpellcorrectServer::onMessage(const TcpConnectionPtr & conn)
{
	cout << "onMessage...." << endl;
	string msg = conn->receive();
	cout << ">> receive msg from client: " << msg << endl;
	//业务逻辑的处理要交给线程池处理
	//decode
	//compute
	//encode
	//::sleep(2);//碰到需要长时间的处理时，响应速度会降下来
	//conn->send(msg);
	MyTask task(msg, conn);

	threadpool_.addTask(std::bind(&MyTask::process, task));
}

void SpellcorrectServer::onClose(const mm::TcpConnectionPtr & conn)
{
	cout << "onClose...." << endl;
	cout << conn->toString() << " has closed!" << endl;
}

using namespace std::placeholders;
void SpellcorrectServer::start()
{
	server_.setConnectionCallback(bind(&SpellcorrectServer::onConnection,this,_1));
	server_.setMessageCallback(bind(&SpellcorrectServer::onMessage,this,_1));
	server_.setCloseCallback(bind(&SpellcorrectServer::onClose,this,_1));
    
    server_.start();
}

}//end of namespace mm

