#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")

class ServerCommunication {
public:
	ServerCommunication();
	~ServerCommunication();
	int Send(char* buf, int len);
	char* Recv(int len);
private:
	SOCKET sock;
};