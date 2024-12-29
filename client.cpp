#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <vector>

#include <iostream>

template<typename ConnectionPorcessor>
class CTCPClient : public ConnectionPorcessor
{
public:
    CTCPClient(int nServerPort, const char *strServerIP)
    {
	m_nServerPort = nServerPort;

	int nlength = strlen(strServerIP);
	m_strServerIP = new char [nlength + 1];
	strcpy(m_strServerIP, strServerIP);
    }

    virtual ~CTCPClient()
    {
	delete [] m_strServerIP;
    }
    
private:
	vector<Vehicle> vehiclePool;
	Queue<Task> taskQueue;
public:
	// accept tasks
	void acceptTask(Task task) {
    taskQueue.push(task);
    std::cout << "Task accepted: " << task.getTaskID() << std::endl;
}
	// assign vehicle
	void dispatchVehicle(Task task) {
   
    if (!vehiclePool.empty()) {
        Vehicle& vehicle = vehiclePool.front();
        vehicle.switchStatus("busy");
        std::cout << "Vehicle " << vehicle.getVehicleID() << " dispatched for task " << task.getTaskID() << std::endl;
    } else {
        std::cout << "No available vehicles for task " << task.getTaskID() << std::endl;
    }
}
void dispatchVehicle(Task task) {
    if (!vehiclePool.empty()) {
        Vehicle& vehicle = vehiclePool.front();
        vehicle.switchStatus("busy");
        std::cout << "Vehicle " << vehicle.getVehicleID() << " dispatched for task " << task.getTaskID() << std::endl;
    } else {
        std::cout << "No available vehicles for task " << task.getTaskID() << std::endl;
    }
}
	// report
	void reportTaskCompletion(Task task) {
    std::cout << "Reporting task completion for task " << task.getTaskID() << std::endl;
}

public:
    int Run()
    {
	int nClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == nClientSocket)
	{
	    std::cout << "socket error" << std::endl;
	    return -1;
	}

	sockaddr_in ServerAddress;
	memset(&ServerAddress, 0, sizeof(sockaddr_in));
	ServerAddress.sin_family = AF_INET;
	if(::inet_pton(AF_INET, m_strServerIP, &ServerAddress.sin_addr) != 1)
	{
	    std::cout << "inet_pton error" << std::endl;
	    ::close(nClientSocket);
	    return -1;
	}

	ServerAddress.sin_port = htons(m_nServerPort);

	if(::connect(nClientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1)
	{
	    std::cout << "connect error" << std::endl;
	    ::close(nClientSocket);
	    return -1;
	}

	ConnectionPorcessor *pProcess = static_cast<ConnectionPorcessor *>(this);
	pProcess->ClientFunction(nClientSocket);

	::close(nClientSocket);

	return 0;
    }

private:
    int m_nServerPort;
    char *m_strServerIP;
};

class CMyTCPClient
{
public:
    CMyTCPClient()
    {
    }

    virtual ~CMyTCPClient()
    {
    }

    void ClientFunction(int nConnectedSocket) {
    char buf[1024];
size_t messageLength = strlen(message);
send(nConnectedSocket, message, messageLength, 0);

char buffer[50000];
ssize_t bytesRead = recv(nConnectedSocket, buffer, sizeof(buffer) - 1, 0);
if (bytesRead > 0) {
    buffer[bytesRead] = '\0'; // 确保字符串结束
} else if (bytesRead < 0) {
    std::cerr << "Error in recv function" << std::endl;
}

std::vector<std::string> strs = split(buffer);
std::cout << "Received task: " << buffer << std::endl;
if (std::strstr(buffer, "update")) {
    // 获取保存地址
    std::string save_path = "plugin/" + strs[1];
    // 表示客户端可以传输so文件了
    message = "ready to update!\n";
    send(nConnectedSocket, message, messageLength, 0);
    // 接收文件大小
    size_t fileSize;
    recv(nConnectedSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
    // 创建缓冲区接收文件内容
    std::vector<char> fileContent(fileSize);
    recv(nConnectedSocket, fileContent.data(), fileSize, 0);
    std::ofstream outFile(save_path.c_str(), std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to create output file" << std::endl;
    }
}

    // 发送任务信息到服务器
    ::write(nConnectedSocket, taskInfo.c_str(), taskInfo.size());

    // 接收服务器的响应
    int bytesRead = ::read(nConnectedSocket, buf, sizeof(buf) - 1);
    if (bytesRead > 0) {
        buf[bytesRead] = '\0';
        std::cout << "Server response: " << buf << std::endl;
    }

    ::close(nConnectedSocket);
}
};

int main()
{
    CTCPClient<CMyTCPClient> client(4000, "127.0.0.1");
    client.Run();

    return 0;
}

