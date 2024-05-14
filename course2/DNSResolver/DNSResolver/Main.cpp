#pragma comment(lib, "Ws2_32.lib")

#include <ws2tcpip.h>
#include <WinSock2.h>
#include <stdio.h>
#include <vector>
#include <locale>

BOOL DomainToIPs(WCHAR *szDomain, std::vector<IN_ADDR> &addrVec)
{
	ADDRINFOW *pAddrInfo;
	SOCKADDR_IN *pSockAddr;
	if (GetAddrInfo(szDomain, L"0", NULL, &pAddrInfo) != 0)
	{
		return FALSE;
	}


	ADDRINFOW *nextAddrInfo = pAddrInfo;
	while (nextAddrInfo != NULL)
	{
		pSockAddr = (SOCKADDR_IN *)nextAddrInfo->ai_addr;
		addrVec.push_back(pSockAddr->sin_addr);
		nextAddrInfo = nextAddrInfo->ai_next;
	}
	FreeAddrInfo(pAddrInfo);
	return TRUE;
}

BOOL DomainToIP(WCHAR *szDomain, IN_ADDR &addr)
{
	std::vector<IN_ADDR> addrVec;
	if (!DomainToIPs(szDomain, addrVec))
	{
		return FALSE;
	}

	size_t s = addrVec.size();
	short randNum = rand() % s;
	addr = addrVec[randNum];
	return TRUE;
}

int wmain()
{
	std::locale::global(std::locale("Korean"));

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		int errCode = WSAGetLastError();
		wprintf(L"WSAStartup Error : Code %d\n", errCode);
		return 1;
	}

	wprintf(L"������ IP ��ȸ ���α׷�\n");

	while (true)
	{
		wprintf(L"������ �ּҸ� �Է� : ");
		WCHAR domainName[100];
		wscanf_s(L"%s", domainName, 100);

		std::vector<IN_ADDR> addrVec;
		if (!DomainToIPs(domainName, addrVec))
		{
			wprintf(L"%s ��ȸ ����\n", domainName);
		}
		else
		{
			wprintf(L"%s �� ���� IP �ּ�\n", domainName);

			for (IN_ADDR addr : addrVec)
			{
				wprintf(L"%d.%d.%d.%d\n", addr.S_un.S_un_b.s_b1, addr.S_un.S_un_b.s_b2, addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b4);
			}

			// �������� ������ �ּ� ��ȯ

			wprintf(L"%s �� �������� ������ ���� IP �ּ�\n", domainName);

			IN_ADDR addr;
			DomainToIP(domainName, addr);
			wprintf(L"%d.%d.%d.%d\n", addr.S_un.S_un_b.s_b1, addr.S_un.S_un_b.s_b2, addr.S_un.S_un_b.s_b3, addr.S_un.S_un_b.s_b4);
		}
	}


	WSACleanup();
	return 0;
}