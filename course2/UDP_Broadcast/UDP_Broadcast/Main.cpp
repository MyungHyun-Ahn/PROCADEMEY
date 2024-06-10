#pragma  comment(lib, "ws2_32")
#pragma  comment(lib, "Winmm")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale>


#define SERVER_IP L"255.255.255.255"
#define START_PORT 10001

#define BUFSIZE 512

int wmain()
{
	std::locale::global(std::locale("Korean"));
	timeBeginPeriod(1);

	wprintf(L"Start ...\n");

	int retVal;
	int errVal;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	for (int i = 0; i < 99; ++i)
	{
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == INVALID_SOCKET)
			return 1;

		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		InetPton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
		serverAddr.sin_port = htons(START_PORT + i);

		u_long on = 1;
		retVal = ioctlsocket(sock, FIONBIO, &on);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			wprintf(L"ioctlsocket() Error : %d\n", errVal);
			return 1;
		}

		BOOL bEnable = TRUE;
		retVal = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) & bEnable, sizeof(bEnable));
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			wprintf(L"setsockopt() Error : %d\n", errVal);
			return 1;
		}

		BYTE sendData[10];
		sendData[0] = 0xff;
		sendData[1] = 0xee;
		sendData[2] = 0xdd;
		sendData[3] = 0xaa;
		sendData[4] = 0x00;
		sendData[5] = 0x99;
		sendData[6] = 0x77;
		sendData[7] = 0x55;
		sendData[8] = 0x33;
		sendData[9] = 0x11;

		retVal = sendto(sock, (char *)sendData, 10, 0, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			if (errVal != WSAEWOULDBLOCK)
			{
				// 진짜 에러
				wprintf(L"sendto error : %d\n", errVal);

				return 1;
			}
		}

		wprintf(L"%d 전송 시작 \n", START_PORT + i);

		int startTime = timeGetTime();
		while (true)
		{
			SOCKADDR_IN peerAddr;
			int addrlen = sizeof(peerAddr);
			char buf[BUFSIZE + 1];

			WCHAR szServerIP[16] = { 0 };
			short serverPort;

			WCHAR *roomName;


			retVal = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&peerAddr, &addrlen);
			if (retVal == SOCKET_ERROR)
			{
				errVal = WSAGetLastError();
				if (errVal != WSAEWOULDBLOCK)
				{
					// 진짜 에러
					wprintf(L"recvfrom error : %d\n", errVal);

					return 1;
				}

				goto GO;
			}

			serverPort = ntohs(peerAddr.sin_port);

			InetNtop(AF_INET, &peerAddr.sin_addr, szServerIP, 16);
			wprintf(L"%s:%d\n", szServerIP, serverPort);

			roomName = (WCHAR *)buf;
			*(roomName + retVal / 2) = L'\0';
			wprintf(L"방 이름 : %s\n", (WCHAR *)buf);
			break;

		GO:

			// 200 밀리 대기 - 종료
			if (int time = timeGetTime(); time - startTime >= 500)
				break;
		}
	}

	WSACleanup();

	timeEndPeriod(1);
	return 0;
}