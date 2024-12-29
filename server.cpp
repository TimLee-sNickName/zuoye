#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <thread> // 包含线程库
#include <iostream>

template<typename ConnectionProcessor>
class CTCPServer : public ConnectionProcessor {
public:
    CTCPServer(int nServerPort, int nLengthOfQueueOfListen = 100, const char *strBoundIP = NULL) {
        m_nServerPort = nServerPort;
        m_nLengthOfQueueOfListen = nLengthOfQueueOfListen;

        if (NULL == strBoundIP) {
            m_strBoundIP = NULL;
        } else {
            int length = strlen(strBoundIP);
            m_strBoundIP = new char[length + 1];
            memcpy(m_strBoundIP, strBoundIP, length + 1);
        }
    }

    virtual ~CTCPServer() {
        if (m_strBoundIP != NULL) {
            delete[] m_strBoundIP;
        }
    }

public:
    int Run() {
        int nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == nListenSocket) {
            std::cout << "socket error" << std::endl;
            return -1;
        }

        sockaddr_in ServerAddress;
        memset(&ServerAddress, 0, sizeof(sockaddr_in));
        ServerAddress.sin_family = AF_INET;

        if (NULL == m_strBoundIP) {
            ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        } else {
            if (::inet_pton(AF_INET, m_strBoundIP, &ServerAddress.sin_addr) != 1) {
                std::cout << "inet_pton error" << std::endl;
                ::close(nListenSocket);
                return -1;
            }
        }

        ServerAddress.sin_port = htons(m_nServerPort);

        if (::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) == -1) {
            std::cout << "bind error" << std::endl;
            ::close(nListenSocket);
            return -1;
        }

        if (::listen(nListenSocket, m_nLengthOfQueueOfListen) == -1) {
            std::cout << "listen error" << std::endl;
            ::close(nListenSocket);
            return -1;
        }

        sockaddr_in ClientAddress;
        socklen_t LengthOfClientAddress = sizeof(sockaddr_in);
        std::vector<std::thread> threads; // 线程容器

        while (true) {
            int nConnectedSocket = ::accept(nListenSocket, (sockaddr *)&ClientAddress, &LengthOfClientAddress);
            if (-1 == nConnectedSocket) {
                std::cout << "accept error" << std::endl;
                break; // 退出循环
            }

            std::thread newThread(&CTCPServer::ServerFunction, this, nConnectedSocket);
            threads.emplace_back(std::move(newThread)); // 将新线程添加到容器中
        }

        // 等待所有线程运行完毕，然后关闭监听套接字
        for (auto &thread : threads) {
            thread.join();
        }
        ::close(nListenSocket);

        return 0;
    }

private:
    int m_nServerPort;
    char* m_strBoundIP;
    int m_nLengthOfQueueOfListen;

    void ServerFunction(int nConnectedSocket) {
        char buf[1024];
        int bytesRead = ::read(nConnectedSocket, buf, sizeof(buf) - 1);
        if (bytesRead > 0) {
            buf[bytesRead] = '\0';
            std::cout << "Received task: " << buf << std::endl;

            // 处理接收到的任务...
            // 解析任务信息
        std::istringstream iss(buf);
        std::string command, taskID, status;
        iss >> command >> taskID >> status;

        // 根据任务状态执行操作
        if (command == "TASK" && status == "new") {
            // 假设 assignTask 是分配任务的函数
            assignTask(taskID);
            std::string response = "TASK:" + taskID + ",assigned";
            ::write(nConnectedSocket, response.c_str(), response.size());
        } else {
            std::string response = "TASK:" + taskID + ",error";
            ::write(nConnectedSocket, response.c_str(), response.size());
        }
        }
        ::close(nConnectedSocket);
    }
};

// CMyTCPServer 类和其他相关代码...
