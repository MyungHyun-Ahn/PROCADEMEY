#include <wchar.h>
#include <stdio.h>
#include <windows.h>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include "CMyFileLoader.h"

int main()
{
	CMyFileLoader cFile;
	cFile.Parse(L"ServerConfig.m");

	INT a;
	cFile.Load(L"ClassName1", L"Key2", &a);

	std::string str1;
	cFile.Load(L"Server", L"IP", &str1);

	USHORT port;
	cFile.Load(L"Server", L"Port", &port);

	std::wstring str2;
	cFile.Load(L"Server", L"IP", &str2);

	return 0;
}