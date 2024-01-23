#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

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
		*context = str;
	}
	
	size_t nextStr = tcslen(*context);
	if (nextStr <= 0)
	{
		return nullptr;
	}

	TCHAR *retStr = *context;
	*context = *context + nextStr + 1;
	return retStr;
}

int main()
{
	TCHAR str1[100] = TEXT("Hello ");
	const TCHAR *str2 = TEXT("World");

	size_t c1 = _tcslen(str1);
	size_t c2 = tcslen(str1);

	tcscat(str1, str2);

	TCHAR str3[100] = TEXT("I love you");
	const TCHAR *seps = TEXT(" ,\t\n");
	TCHAR *token = nullptr;
	TCHAR *word = tcstok(str3, seps, &token);
	while (word != nullptr)
	{
		_tprintf(TEXT("%s\n"), word);
		word = tcstok(NULL, seps, &token);
	}

	return 0;
}