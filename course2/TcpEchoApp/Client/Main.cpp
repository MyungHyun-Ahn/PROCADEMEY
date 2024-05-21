#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale>

#define SERVER_IP L"127.0.0.1"
constexpr int SERVER_PORT = 12345;
constexpr int BUF_SIZE = 512;

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int wmain()
{
	std::locale::global(std::locale("Korean"));

	int retVal;
	int errVal;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		errVal = WSAGetLastError();
		wprintf(L"socket() Error : %d\n", errVal);
		return 1;
	}

	// connect()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);
	retVal = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"connect() Error : %d\n", errVal);
		return 1;
	}

	WCHAR szBuf[BUF_SIZE + 1];
	int len;

	// ������ ������ ���
	while (1)
	{
		// ������ �Է�
		wprintf(L"\n[���� ������] ");
		if (fgetws(szBuf, BUF_SIZE + 1, stdin) == NULL)
			break;

		// '\n' ���� ����
		len = wcslen(szBuf);
		if (szBuf[len - 1] == L'\n')
			szBuf[len - 1] = '\0';
		if (wcslen(szBuf) == 0)
			break;

		// ������ ������
		retVal = send(sock, (char *)szBuf, wcslen(szBuf) * sizeof(WCHAR), 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			wprintf(L"send() Error : %d\n", errVal);
			break;
		}

		wprintf(L"[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� ���½��ϴ�. \n", retVal);

		// ������ �ޱ�
		retVal = recvn(sock, (char *)szBuf, retVal, 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			wprintf(L"recv() Error : %d\n", errVal);
			break;
		}
		else if (retVal == 0)
			break;

		// ���� ������ ���
		szBuf[retVal / 2 + 1] = L'\0';
		wprintf(L"[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ�.\n", retVal);
		wprintf(L"[���� ������] %s\n", szBuf);
	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}