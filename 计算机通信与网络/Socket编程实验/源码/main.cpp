#pragma once
#include "winsock2.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

// 设置接收缓冲区
char recvBuf[4096];//请求报文

void sendfile(char recvBuf[4096], SOCKET sessionSocket, char path[30])
{
	//解析接收到的报文，处理需要发送的文件，path为文件所在路径
	char filetype[5], filename[200], filepath[500], content_type[300] = "Content-type:";
	memset(filetype, 0, sizeof(filetype));
	memset(filename, 0, sizeof(filename));
	memset(filepath, 0, sizeof(filepath));
	//下面开始提取文件名和文件路径
	int i = 0, j = 0;
	while (recvBuf[i++] != '/')
		;//忽略前面的GET
	while (recvBuf[i] != '.')
		filename[j++] = recvBuf[i++];//取出文件名（不包含文件类型后缀）
	filename[j] = '.';
	j = 0;
	while (recvBuf[++i] != ' ')
		filetype[j++] = recvBuf[i];
	strcat(filename, filetype);//拼成完整的文件名
	strcpy(filepath, path);
	strcat(filepath, "\\");
	strcat(filepath, filename);				//获取请求的文件名及其文件类型
	if (strcmp(filetype, "html") == 0)
		strcat(content_type, "text/html\r\n");
	if (!strcmp(filetype, "jpg") == 0)
		strcat(content_type, "image/jpg\r\n");
	if (!strcmp(filetype, "png") == 0)
		strcat(content_type, "image/png\r\n");
	if (!strcmp(filetype, "txt") == 0)
		strcat(content_type, "text/txt\r\n");

	char head[] = "HTTP/1.1 200 OK\r\n";
	char not_found[] = "HTTP/1.1 404 NOT FOUND\r\n";
	char forbidden[] = "HTTP.1.1 403 FORBIDDEN\r\n";

	if (!strcmp(filename, "denytest.jpg"))
	{
		send(sessionSocket, forbidden, strlen(forbidden), 0);
		send(sessionSocket, "Content-type:text/plain\r\n", strlen("Content-type:text/plain\r\n"), 0);
		send(sessionSocket, "\r\n", 2, 0);
		//构造响应报文
		printf("Result: Access denied, returning 403\n\n");
	}
	else {
		FILE* fp = fopen(filepath, "rb");
		if (!fp) {
			send(sessionSocket, not_found, strlen(not_found), 0);
			send(sessionSocket, "\r\n", 2, 0);
			//构造响应报文
			printf("Result: The requested file was not found, returning 404\n\n");
		}
		else
		{
			send(sessionSocket, head, strlen(head), 0);
			send(sessionSocket, "\r\n", 2, 0);
			fseek(fp, 0L, SEEK_END);
			int flen = ftell(fp);
			char* p = (char*)malloc(flen + 1);
			fseek(fp, 0L, SEEK_SET);
			fread(p, flen, 1, fp);
			send(sessionSocket, p, flen, 0);
			//构造响应报文
			printf("Result: Send %s to client\n\n", filename);
			fp = NULL;
		}
	}
	return;
}

int main(void)
{
	WSADATA wsaData;
	/*
		select()机制中提供的fd_set的数据结构，实际上是long类型的数组，
		每一个数组元素都能与一打开的文件句柄（不管是socket句柄，还是其他文件或命名管道或设备句柄）建立联系，建立联系的工作由程序员完成.
		当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执行了select()的进程哪个socket或文件句柄发生了可读或可写事件。
	*/

	short port;
	char ip[16] = "0", path[300] = "0";//配置监听端口、ip和主目录
	printf("Input listening ip: ");
	scanf("%s", ip);
	printf("Input port: ");
	scanf("%hd", &port);
	printf("Input path: ");
	scanf("%s", path);

	int nRc = WSAStartup(0x0202, &wsaData);

	if (nRc)
	{
		printf("Winsock  startup failed with error!\n");
	}

	if (wsaData.wVersion != 0x0202)
	{
		printf("Winsock version is not correct!\n");
	}

	printf("Winsock  startup Ok!\n");

	// 监听socket
	SOCKET srvSocket;

	// 服务器地址和客户端地址
	sockaddr_in addr, clientAddr;

	// 会话socket，负责和client进程通信
	SOCKET sessionSocket;

	// ip地址长度
	int addrLen;

	// 创建监听socket
	srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSocket != INVALID_SOCKET)
		printf("Socket create Ok!\n");

	// 设置服务器的端口和地址
	addr.sin_family = AF_INET;
	// addr.sin_port = htons(5050);
	addr.sin_port = htons(port);
	// addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // 主机上任意一块网卡的IP地址
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

	// binding
	int rtn = bind(srvSocket, (LPSOCKADDR)&addr, sizeof(addr));
	if (rtn != SOCKET_ERROR)
		printf("Socket bind Ok!\n");

	// 监听
	// rtn = listen(srvSocket, 5);
	rtn = listen(srvSocket, SOMAXCONN);//最大监听队列长度
	if (rtn != SOCKET_ERROR)
		printf("Socket listen Ok!\n");

	clientAddr.sin_family = AF_INET;
	addrLen = sizeof(clientAddr);

	while (true)
	{
		memset(recvBuf, '\0', 4096);
		sessionSocket = accept(srvSocket, (LPSOCKADDR)&clientAddr, &addrLen);
		if (sessionSocket == INVALID_SOCKET)
			printf("Connect failed\n");
		printf("Received request message from client %s:%u\n", inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));
		rtn = recv(sessionSocket, recvBuf, 4096, 0);
		if (!rtn)
			printf("Failed to receive data\n");
		printf("Received %d bytes from client:\n%s", strlen(recvBuf), recvBuf);
		sendfile(recvBuf, sessionSocket, path);
		closesocket(sessionSocket);
	}
	closesocket(srvSocket);
	WSACleanup();
	return 0;
}