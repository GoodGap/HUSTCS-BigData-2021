#pragma once
#include "winsock2.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

// ���ý��ջ�����
char recvBuf[4096];//������

void sendfile(char recvBuf[4096], SOCKET sessionSocket, char path[30])
{
	//�������յ��ı��ģ�������Ҫ���͵��ļ���pathΪ�ļ�����·��
	char filetype[5], filename[200], filepath[500], content_type[300] = "Content-type:";
	memset(filetype, 0, sizeof(filetype));
	memset(filename, 0, sizeof(filename));
	memset(filepath, 0, sizeof(filepath));
	//���濪ʼ��ȡ�ļ������ļ�·��
	int i = 0, j = 0;
	while (recvBuf[i++] != '/')
		;//����ǰ���GET
	while (recvBuf[i] != '.')
		filename[j++] = recvBuf[i++];//ȡ���ļ������������ļ����ͺ�׺��
	filename[j] = '.';
	j = 0;
	while (recvBuf[++i] != ' ')
		filetype[j++] = recvBuf[i];
	strcat(filename, filetype);//ƴ���������ļ���
	strcpy(filepath, path);
	strcat(filepath, "\\");
	strcat(filepath, filename);				//��ȡ������ļ��������ļ�����
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
		//������Ӧ����
		printf("Result: Access denied, returning 403\n\n");
	}
	else {
		FILE* fp = fopen(filepath, "rb");
		if (!fp) {
			send(sessionSocket, not_found, strlen(not_found), 0);
			send(sessionSocket, "\r\n", 2, 0);
			//������Ӧ����
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
			//������Ӧ����
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
		select()�������ṩ��fd_set�����ݽṹ��ʵ������long���͵����飬
		ÿһ������Ԫ�ض�����һ�򿪵��ļ������������socket��������������ļ��������ܵ����豸�����������ϵ��������ϵ�Ĺ����ɳ���Ա���.
		������select()ʱ�����ں˸���IO״̬�޸�fd_set�����ݣ��ɴ���ִ֪ͨ����select()�Ľ����ĸ�socket���ļ���������˿ɶ����д�¼���
	*/

	short port;
	char ip[16] = "0", path[300] = "0";//���ü����˿ڡ�ip����Ŀ¼
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

	// ����socket
	SOCKET srvSocket;

	// ��������ַ�Ϳͻ��˵�ַ
	sockaddr_in addr, clientAddr;

	// �Ựsocket�������client����ͨ��
	SOCKET sessionSocket;

	// ip��ַ����
	int addrLen;

	// ��������socket
	srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSocket != INVALID_SOCKET)
		printf("Socket create Ok!\n");

	// ���÷������Ķ˿ں͵�ַ
	addr.sin_family = AF_INET;
	// addr.sin_port = htons(5050);
	addr.sin_port = htons(port);
	// addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // ����������һ��������IP��ַ
	addr.sin_addr.S_un.S_addr = inet_addr(ip);

	// binding
	int rtn = bind(srvSocket, (LPSOCKADDR)&addr, sizeof(addr));
	if (rtn != SOCKET_ERROR)
		printf("Socket bind Ok!\n");

	// ����
	// rtn = listen(srvSocket, 5);
	rtn = listen(srvSocket, SOMAXCONN);//���������г���
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