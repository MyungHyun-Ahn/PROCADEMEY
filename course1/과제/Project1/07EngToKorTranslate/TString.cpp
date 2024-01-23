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
	// ó���� str �ް� �ѹ� ����
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
		str[strSize + 1] = 0xaa; // ������ üũ ���� ����
		str[strSize + 2] = '\0';
		*context = str;
	}

	if (*context == nullptr) // ������ ���ڶ�� Pass
	{
		return nullptr;
	}

	size_t nextStr = tcslen(*context);
	TCHAR *retStr = *context;
	*context = *context + nextStr;
	while (**context == '\0') // H e l l o \0 \0 �̷� ��� �н�
	{
		*context = *context + 1;
	}

	if (**context == 0xaa) // ������ ���� �˻�
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
