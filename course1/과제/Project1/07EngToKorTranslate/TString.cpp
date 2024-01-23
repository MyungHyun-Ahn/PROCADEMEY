#include "pch.h"
#include "TString.h"

size_t tcslen(const TCHAR *str)
{
	size_t strSize = 0;
	while (str[strSize] != '\0')
	{
		strSize++;
	}

	return strSize;
}

TCHAR *tcscat(TCHAR *dest, const TCHAR *source)
{
	size_t destSize = tcslen(dest);
	memcpy(dest + destSize, source, (tcslen(source) + 1) * sizeof(TCHAR));
	return dest;
}

TCHAR *tcstok(TCHAR *str, const TCHAR *delimiters, TCHAR **context)
{
	// 처음에 str 받고 한번 수행
	if (str != NULL)
	{
		size_t strSize = 0;
		while (str[strSize] != '\0')
		{
			for (size_t i = 0; delimiters[i] != '\0'; i++)
			{
				if (str[strSize] == delimiters[i])
				{
					str[strSize] = '\0';
				}
			}
			strSize++;
		}
		str[strSize + 1] = 0xaa; // 마지막 체크 문자 기입
		str[strSize + 2] = '\0';
		*context = str;
	}

	if (*context == nullptr) // 마지막 문자라면 Pass
	{
		return nullptr;
	}

	size_t nextStr = tcslen(*context);
	TCHAR *retStr = *context;
	*context = *context + nextStr;
	while (**context == '\0') // H e l l o \0 \0 이런 경우 패스
	{
		*context = *context + 1;
	}

	if (**context == 0xaa) // 마지막 문자 검사
	{
		*context = nullptr;
	}

	return retStr;
}

int tcscmp(const TCHAR *str1, const TCHAR *str2)
{
	int curser = 0;
	do 
	{
		if (str1[curser] != str2[curser])
			return -1;

		curser++;
	} while (str1[curser] != '\0' || str2[curser] != '\0');
	return 0;
}
