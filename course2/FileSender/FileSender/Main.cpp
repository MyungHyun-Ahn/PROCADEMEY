#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale>

#define SERVER_IP L"106.245.38.108"
constexpr int SERVER_PORT = 10010;
constexpr int BUF_SIZE = 1000; // 100�� ������

// �������� - ���
struct st_PACKET_HEADER
{
	DWORD dwPacketCode;		// 0x11223344 - ��Ŷ Ȯ�� ������
	WCHAR szName[32];		// ���� �̸� �����
	WCHAR szFileName[128];  // ���� �̸�
	int iFileSize;			// ���� ũ��
};

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

char *GenPacket(WCHAR *fileName, int &fileSize)
{
	// ��Ŷ ��� ����
	st_PACKET_HEADER ph;
	ph.dwPacketCode = 0x11223344;
	wmemcpy_s(ph.szName, 32, L"MyunghyunAhn", 13);
	wmemcpy_s(ph.szFileName, 128, fileName, wcslen(fileName) + sizeof(WCHAR));

	FILE *pFile;
	errno_t err = _wfopen_s(&pFile, fileName, L"rb");
	if (err)
	{
		// error
		return nullptr;
	}

	fseek(pFile, 0, SEEK_END);
	ph.iFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	fileSize = ph.iFileSize + sizeof(st_PACKET_HEADER);
	char *buffer = new char[ph.iFileSize + sizeof(st_PACKET_HEADER)];
	memcpy_s(buffer, sizeof(st_PACKET_HEADER), &ph, sizeof(st_PACKET_HEADER));

	int readSize = fread_s(buffer + sizeof(st_PACKET_HEADER), ph.iFileSize, sizeof(char), ph.iFileSize, pFile);
	if (readSize != ph.iFileSize)
	{
		wprintf(L"error - fread_s : %s \n", fileName);
		return nullptr;
	}

	fclose(pFile);

	return buffer;

	// {
	// 	FILE *pFile;
	// 	errno_t err = _wfopen_s(&pFile, L"Test2.png", L"wb");
	// 	if (err)
	// 	{
	// 		// error
	// 		return nullptr;
	// 	}
	// 
	// 	fwrite(buffer + sizeof(st_PACKET_HEADER), readSize, sizeof(char), pFile);
	// 	fclose(pFile);
	// }
}

int sendn(SOCKET s, char *buffer, int bufSize)
{
	int retVal;
	int errVal;
	if (bufSize < 0)
		return false;

	// 1000���� Ŭ ������ ����
	while (bufSize > 1000)
	{
		retVal = send(s, buffer, 1000, 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			wprintf(L"send() Error : %d\n", errVal);
			return false;
		}
		bufSize -= retVal;
		buffer += retVal;
	}

	// ���� bufSize ��ŭ ����
	retVal = send(s, buffer, bufSize, 0);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"send() Error : %d\n", errVal);
		return false;
	}

	return true;
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
	// ������ �Է�
	WCHAR fileName[32] = L"Procademy.png";
	int fileSize;
	char *packetBuffer = GenPacket(fileName, fileSize);

	// ������ ������ - 1000 �� ���
	retVal = sendn(sock, packetBuffer, fileSize);
	if (retVal == false)
	{
		errVal = WSAGetLastError();
		wprintf(L"send() Error : %d\n", errVal);
		return 1;
	}

	// ������ �ޱ�
	retVal = recvn(sock, (char *)szBuf, sizeof(int), 0);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"recv() Error : %d\n", errVal);
		return 1;
	}
	else if (retVal == 0)
		return 1;

	// ���� ������ ���
	wprintf(L"[���� ������] %x\n", *(int *)szBuf);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}