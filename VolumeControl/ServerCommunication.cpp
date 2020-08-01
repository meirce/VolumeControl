#include "ServerCommunication.h"
#define DEFAULT_PORT "27015"

ServerCommunication::ServerCommunication()
{
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		exit(1);
	}

	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;


	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);
	
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		exit(1);
	}

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		
	if (iResult != 0) {
		printf("Failed to connect with error code: %ld\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}

	this->sock = ConnectSocket;

}

ServerCommunication::~ServerCommunication()
{
	closesocket(this->sock);
}

int ServerCommunication::Send(char * buf, int len)
{
	
	int result = send(this->sock, buf, len, 0);
	if (result < 0) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(this->sock);
		WSACleanup();
	}
	return result;
}

char * ServerCommunication::Recv(int len)
{
	char* buf = new char[len];
	int result = recv(this->sock, buf, len, 0);
	if (result < 0) {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(this->sock);
		WSACleanup();
		return NULL;
	}
	return buf;
}
