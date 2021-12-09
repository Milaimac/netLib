  /*
 * @Date: 2021-11-27 21:49:47
 * @LastEditors: kafier
 * @LastEditTime: 2021-12-09 18:19:42
 */
#include <getopt.h>
#include <string>
#include "EventLoop.h"
#include "Server.h"
#include "base/Logging.h"

int main(int argc, char *argv[])
{
    int threadNum = 4;
    int port = 80;
    std::string logPath = "./WebServer.log";

    // parse args
    int opt;
    const char *str = "t:l:p:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 't':
        {
            threadNum = atoi(optarg);
            break;
        }
        case 'l':
        {
            logPath = optarg;
            if (logPath.size() < 2 || optarg[0] != '/')
            {
                printf("logPath should start with \"/\"\n");
                abort();
            }
            break;
        }
        case 'p':
        {
            port = atoi(optarg);
            break;
        }
        default:
            break;
        }
    }
    Logger::setLogFileName(logPath);
// STL库在多线程上应用
#ifndef _PTHREADS
    LOG << "_PTHREADS is not defined !";
#endif
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();
    return 0;
}
