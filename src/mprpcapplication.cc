#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc,char **argv)
{
    if(argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1)   //使用getopt解析命令行参数
    {
        switch(c)
        {
            case 'i':
                config_file = optarg;   //获取配置文件
                break;
            case '?':   // getopt遇到不认识的选项时返回'?'
                {
                    ShowArgsHelp();
                    exit(EXIT_FAILURE);
                }
            case ':':   //getopt缺少选项参数时返回':'
                {
                    ShowArgsHelp();
                    exit(EXIT_FAILURE);
                }
            default:
                break;
        }
    }

    //开始加载配置文件  rpcsercer_ip=   rpcserver_port=  zookeeper_ip=   zookepper_port=
    m_config.LoadConfigFile(config_file.c_str());

    // std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
    // std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
    // std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
    // std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return m_config;
}
