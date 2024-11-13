#include <wchar.h>
#include <stdio.h>
#include <windows.h>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include "CMyFileLoader.h"

VOID CMyFileLoader::Parse(const WCHAR *fileName)
{
	int retVal;

	FILE *pFile = _wfsopen(fileName, L"r, ccs = UTF-16LE", _SH_DENYWR);
	if (pFile == nullptr)
	{
		wprintf(L"file open fail, errorCode = %d\n", GetLastError());
		return;
	}

	size_t fileBytes;
	fseek(pFile, 0, SEEK_END);
	fileBytes = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	WCHAR *fileBuffer = new WCHAR[fileBytes / 2]; // wchar니깐
	retVal = fread_s(fileBuffer, fileBytes, sizeof(char), fileBytes, pFile);
	if (retVal == 0)
	{
		// error
		return;
	}

	fclose(pFile);

	fileBuffer[retVal / sizeof(WCHAR)] = L'\0';

	std::wstring wStr(fileBuffer);

	// wstring 으로 만들었다면 파일 버퍼 삭제
	delete[] fileBuffer;

	// \t\r 띄워쓰기 제거
	wStr.erase(remove(wStr.begin(), wStr.end(), L'\t'), wStr.end());
	wStr.erase(remove(wStr.begin(), wStr.end(), L' '), wStr.end());

	// 주석 삭제 "//"
	{
		size_t xPos = wStr.find(L"//");
		while (xPos != std::wstring::npos)
		{
			// nPos 부터
			int yPos;
			for (yPos = xPos; yPos < wStr.size(); yPos++)
			{
				if (wStr[yPos] == L'\n')
					break;
			}

			// \n 문자 1개 추가 + 1
			wStr.erase(xPos, yPos - xPos + 1);

			// 여기도 지웠으니 ok
			xPos = wStr.find(L"//");
		}
	}
	// 한줄 주석을 지웠으면 \n 또한 지워도 됨
	wStr.erase(remove(wStr.begin(), wStr.end(), L'\n'), wStr.end());

	// /* ~ */ 제거
	{
		size_t xPos = wStr.find(L"/*");
		size_t yPos = wStr.find(L"*/");
		while (xPos != std::wstring::npos)
		{
			// */로 닫지 않았다면 이후 모든 것을 지워버림
			if (yPos == std::wstring::npos)
				yPos = wStr.size() - 1;

			// */ 문자 2개니깐 + 2
			wStr.erase(xPos, yPos - xPos + 2);

			// 여긴 지웠으니깐 ok
			xPos = wStr.find(L"/*", xPos);
			yPos = wStr.find(L"*/", yPos);
		}
	}

	// 모든 Key Value를 std::wstring으로 저장
	//	* Get 계열 함수에서 원하는 타입으로 변환
	
	// ClassName 무조건 :으로 끝남
	{
		size_t keyStart = wStr.find(L"-"); // 첫 -
		size_t valueEnd = 0;
		size_t xPos = 1; // wStr의 맨 앞 스킵
		size_t yPos = 0;
		size_t nextYPos = wStr.find(L":");
		while (true)
		{
			yPos = nextYPos;
			if (yPos == std::wstring::npos)
				break;

			// 클래스 이름
			std::wstring className = wStr.substr(xPos, yPos - xPos);
			
			// 이 뒤에 오는 것은 - Key = Value;
			// -를 찾음
			nextYPos = wStr.find(L":", nextYPos + 1);
			while (keyStart != std::wstring::npos)
			{
				// 다음 className 끝보다 keyStart가 앞지른 경우
				if (nextYPos < keyStart)
					break;

				size_t keyEnd = wStr.find(L"=", keyStart);
				keyStart++;
				std::wstring keyStr = wStr.substr(keyStart, keyEnd - keyStart);

				size_t valueStart = keyEnd + 1;
				valueEnd = wStr.find(L";", valueEnd + 1);
				std::wstring valueStr = wStr.substr(valueStart, valueEnd - valueStart);

				m_parsedDatas[className].insert(std::make_pair(keyStr, valueStr));

				keyStart = wStr.find(L"-", keyStart);
			}

			xPos = valueEnd + 1;
		}
	}

	return;
}
