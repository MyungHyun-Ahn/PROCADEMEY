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
			// 10048 : �̹� ����ϰ� �ִ� �ּ�
			// 10055 : socket buffer full
			// 10061 : backlog ť�� �� �� ��
			// SOMAXCONN : 200
			// SOMAXCONN_HINT(N) : ���� ������ ��ŭ - �׷��� ���� ��Ʈ ���� ���� �ٸ�

			// ���� �ذ� ����
			// 1. SOMAXCONN : 200�� ���� ���� Ȯ��
			// 2. SOMAXCONN_HINT(65535) : 13600�� �뿡�� 10055 ���� �߻�
			//		* ��� ���� ��Ʈ ����Ͽ� �߻��� ����
			// 3. SOMAXCONN_HINT(65535) : ���� ��Ʈ ���� ����
			//		* netsh int ipv4 set dynamicport tcp start=5000 num=60000
			//		* 59700�� ���� ����ǰ� 10055 ���� �߻�
			// 4. SOMAXCONN_HINT(65535) : connect �� closesocket
			//		* 59700�� ���� ����ǰ� 10048 ���� �߻�
			// 5. SOMAXCONN_HINT(65535) : linger �ɼ� ����
			//		* 65535�� ���� ����Ǵ� ���� Ȯ��
			//		* LINGER �ɼ��� Ȱ��ȭ �ϰ� l_linger�� 0���� �ϸ� ������ close �ʰ� ���ÿ� ���۵��� ���� ������ ����ó��

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