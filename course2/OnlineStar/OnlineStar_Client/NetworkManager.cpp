#include "pch.h"
#include "Define.h"
#include "Packet.h"
#include "NetworkManager.h"

NetworkManager *g_NetworkMgr;
int g_BufferSize;
char g_Buffer[RECV_BUFFER_SIZE];

NetworkManager::NetworkManager() : serverSocket(INVALID_SOCKET)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		throw 1;
	}
}

NetworkManager::~NetworkManager()
{
	WSACleanup();
}

bool NetworkManager::Connect()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		int errVal = WSAGetLastError();
		printf("socket() Error : %d\n", errVal);
		return false;
	}

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, serverIP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);
	int retVal = connect(serverSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR)
	{
		int errVal = WSAGetLastError();
		printf("connect() Error : %d\n", errVal);
		return false;
	}

	return true;
}

void NetworkManager::RegisterIP(const CHAR *ip)
{
	memcpy_s(serverIP, 16 * sizeof(char), ip, strlen(ip) * sizeof(char));
}

void NetworkManager::CloseSocket()
{
	closesocket(serverSocket);
}

bool NetworkManager::SetNonBlockingOpt()
{
	u_long on = 1;

	int retVal = ioctlsocket(serverSocket, FIONBIO, &on);
	if (retVal == SOCKET_ERROR)
	{
		int errVal = WSAGetLastError();
		printf("ioctlsocket() Error : %d\n", errVal);
		return false;
	}

	return true;
}

bool NetworkManager::Select()
{
	FD_ZERO(&rset);
	FD_SET(serverSocket, &rset);

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	// select()
	int retVal = select(0, &rset, NULL, NULL, &t);
	if (retVal == SOCKET_ERROR)
	{
		int errVal = WSAGetLastError();
		printf("select() Error : %d\n", errVal);
		return false;
	}

	if (retVal == 0)
	{
		return true;
	}
	
	// 받을 데이터가 있는 것
	if (!Recv())
		return false;

	return true;
}

bool NetworkManager::Send(PacketBase *packet)
{
	int retVal = send(serverSocket, (char *)packet, sizeof(PacketBase), 0);
	if (retVal == SOCKET_ERROR)
	{
		int errVal = WSAGetLastError();
		printf("send() Error : %d\n", errVal);
		return false;
	}

	return true;
}

bool NetworkManager::Recv()
{
	// 크게 읽음
	int retVal = recv(serverSocket, g_Buffer, RECV_BUFFER_SIZE, 0);
	if (retVal == SOCKET_ERROR)
	{
		int errVal = WSAGetLastError();
		printf("recv() Error : %d\n", errVal);
		return false;
	}
	else if (retVal == 0)
	{
		int errVal = WSAGetLastError();
		printf("Disconnected : %d\n", errVal);
		return false;
	}

	g_BufferSize = retVal;

	return true;
}
