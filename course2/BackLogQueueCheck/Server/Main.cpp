#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

constexpr int SERVER_PORT = 12345;

int wmain()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		int errCode = WSAGetLastError();
		wprintf(L"WSAStartup Error : Code %d\n", errCode);
		return 1;
	}

	// socket()
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
	{
		int errCode = WSAGetLastError();
		wprintf(L"socket Error : Code %d\n", errCode);
		return 1;
	}

	char optval = 1;
	setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR | SO_LINGER, &optval, sizeof(optval));

	// bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"0.0.0.0", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);

	int retVal;
	retVal = bind(listenSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR)
	{
		int errCode = WSAGetLastError();
		wprintf(L"bind Error : Code %d\n", errCode);
		return 1;
	}

	// listen()
	retVal = listen(listenSock, SOMAXCONN_HINT(60000));
	if (retVal == SOCKET_ERROR)
	{
		int errCode = WSAGetLastError();
		wprintf(L"listen Error : Code %d\n", errCode);
		return 1;
	}

	// 무한 루프
	while (true)
	{
		//wprintf(L"Server is alive");
		int a = 0;
		a++;
	}

	// closesocket()
	closesocket(listenSock);

	WSACleanup();
	return 0;
}