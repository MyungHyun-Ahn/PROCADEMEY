#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale>


constexpr int SERVER_PORT = 12345;
constexpr int BUF_SIZE = 512;

int wmain()
{
	std::locale::global(std::locale("Korean"));

	int retVal;
	int errVal;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
	{
		errVal = WSAGetLastError();
		wprintf(L"socket() Error : %d\n", errVal);
		return 1;
	}

	// bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"0.0.0.0", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);

	retVal = bind(listenSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"bind() Error : %d\n", errVal);
		return 1;
	}

	// listen()
	retVal = listen(listenSock, SOMAXCONN);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"listen() Error : %d\n", errVal);
		return 1;
	}

	// ������ ��ſ� ����� ����
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	WCHAR buf[BUF_SIZE + 1];

	while (1)
	{
		// accept()
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR *)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
		{
			errVal = WSAGetLastError();
			wprintf(L"accept() Error : %d\n", errVal);
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		
		WCHAR szClientIP[16] = { 0, };
		InetNtop(AF_INET, &clientAddr.sin_addr, szClientIP, 16);
		wprintf(L"\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", szClientIP, ntohs(clientAddr.sin_port));
		
		// Ŭ���̾�Ʈ�� ������ ���
		while (1)
		{
			// ������ �ޱ�
			retVal = recv(clientSock, (char *)buf, BUF_SIZE * sizeof(WCHAR), 0);
			if (retVal == SOCKET_ERROR)
			{
				errVal = WSAGetLastError();
				wprintf(L"recv() Error : %d\n", errVal);
				break;
			}
			else if (retVal == 0)
			{
				errVal = WSAGetLastError();
				wprintf(L"recv() Error : %d\n", errVal);
				break;
			}

			// ���� ������ ���
			buf[retVal / 2 + 1] = L'\0';
			wprintf(L"[TCP/%s:%d] %s\n", szClientIP, ntohs(clientAddr.sin_port), buf);

			// ������ ������
			retVal = send(clientSock, (char *)buf, retVal, 0);
			if (retVal == SOCKET_ERROR)
			{
				errVal = WSAGetLastError();
				wprintf(L"send() Error : %d\n", errVal);
				break;
			}
		}

		// closesocket()
		closesocket(clientSock);
		wprintf(L"\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", szClientIP, ntohs(clientAddr.sin_port));
	}

	// closesocket()
	closesocket(listenSock);
	
	// ���� ����
	WSACleanup();
	return 0;
}