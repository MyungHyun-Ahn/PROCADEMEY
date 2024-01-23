#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include <locale.h>

int _tmain()
{
	_tsetlocale(LC_ALL, TEXT("korean"));
	HANDLE fHandle = CreateFile(TEXT("word.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

	TCHAR ReadBuffer[500];
	DWORD readSize = 0;
	if (ReadFile(fHandle, ReadBuffer, 500, &readSize, NULL))
	{
		
	}
	ReadBuffer[(readSize / sizeof(TCHAR))] = '\0';

	TCHAR *token = nullptr;
	const TCHAR *seps = TEXT(" ,\r\n");
	TCHAR *word1 = _tcstok_s(ReadBuffer + 1, seps, &token);
	TCHAR *word2 = _tcstok_s(NULL, seps, &token);
	while (word1 != nullptr || word2 != nullptr)
	{
		_tprintf(TEXT("Load Data : %s, %s\n"), word1, word2);
		word1 = _tcstok_s(NULL, seps, &token);
		word2 = _tcstok_s(NULL, seps, &token);
	}

	CloseHandle(fHandle);

	return 0;
}