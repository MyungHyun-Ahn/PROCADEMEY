#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define SERVER_IP L"127.0.0.1"
constexpr int SERVER_PORT = 12345;

int counter = 0;

int wmain()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		int errCode = WSAGetLastError();
		wprintf(L"WSAStartup Error : Code %d\n", errCode);
		return 1;
	}

	std::vector<SOCKET> sockArr;

	while (true)
	{
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			int errCode = WSAGetLastError();
			wprintf(L"socket Error : Code %d\n", errCode);
			return 1;
		}

		char optval = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_LINGER, &optval, sizeof(optval));

		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		InetPton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
		serverAddr.sin_port = htons(SERVER_PORT);

		int retVal = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
		if (retVal == SOCKET_ERROR)
		{
			int errCode = WSAGetLastError();
			// 10048 : 이미 사용하고 있는 주소
			// 10055 : socket buffer full
			// 10061 : backlog 큐가 꽉 찬 것
			// SOMAXCONN : 200
			// SOMAXCONN_HINT(N) : 숫자 설정한 만큼 - 그런데 가용 포트 수에 따라 다름
			wprintf(L"connect Error : Code %d\n", errCode);
			break;
		}

		counter++;

		wprintf(L"connect success num : %d\n", counter);

		// closesocket(sock);
	}


	wprintf(L"Backlog Queue Size : %d\n", counter);


	WSACleanup();
	return 0;
}