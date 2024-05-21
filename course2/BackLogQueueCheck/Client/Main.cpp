#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define SERVER_IP L"127.0.0.1"
constexpr int SERVER_PORT = 8005;

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

		LINGER ling;
		ling.l_linger = 0;
		ling.l_onoff = 1;
		setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));

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

			// 문제 해결 순서
			// 1. SOMAXCONN : 200개 까지 연결 확인
			// 2. SOMAXCONN_HINT(65535) : 13600개 쯤에서 10055 에러 발생
			//		* 모든 가용 포트 사용하여 발생한 문제
			// 3. SOMAXCONN_HINT(65535) : 가용 포트 범위 증설
			//		* netsh int ipv4 set dynamicport tcp start=5000 num=60000
			//		* 59700개 정도 연결되고 10055 에러 발생
			// 4. SOMAXCONN_HINT(65535) : connect 후 closesocket
			//		* 59700개 정도 연결되고 10048 에러 발생
			// 5. SOMAXCONN_HINT(65535) : linger 옵션 설정
			//		* 65535개 전부 연결되는 것을 확인
			//		* LINGER 옵션을 활성화 하고 l_linger을 0으로 하면 소켓이 close 됨과 동시에 전송되지 않은 데이터 버림처리

			wprintf(L"connect Error : Code %d\n", errCode);
			break;
		}

		counter++;

		wprintf(L"connect success num : %d\n", counter);

		closesocket(sock);
	}


	wprintf(L"Backlog Queue Size : %d\n", counter);


	WSACleanup();
	return 0;
}