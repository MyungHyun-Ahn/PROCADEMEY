#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
const char *KEY = "MyungHyun";
const char MASK = 0xAA;

int main()
{
	char *buffer;
	FILE *pFile;

	char pFileName[256];
	printf("파일 이름을 입력하세요 : ");
	scanf_s("%s", pFileName, 256);

	errno_t err	= fopen_s(&pFile , pFileName, "rb");
	if (pFile == 0)
		return 1;

	fseek(pFile, 0, SEEK_END);
	int fileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	char CheckBuffer[10];
	fread(CheckBuffer, sizeof(char), 10, pFile);

	if (strcmp(CheckBuffer, KEY) != 0)
	{
		// 암호화
		fseek(pFile, 0, SEEK_SET);
		buffer = (char *)malloc(fileSize + 11);
		if (buffer == nullptr)
			return 1;
		buffer[fileSize + 11] = '\0';
		strcpy(buffer, KEY);
		fread(buffer + 10, sizeof(char), fileSize, pFile);
		fclose(pFile);
		pFile = nullptr;

		for (char *c = buffer + 10; *c != '\0'; c++)
		{
			*c ^= MASK;
		}

		fopen_s(&pFile, pFileName, "wb");
		if (pFile == 0)
			return 1;

		fwrite(buffer, sizeof(char), fileSize + 10, pFile);
		fclose(pFile);

		printf("파일 %s의 암호화가 완료되었습니다.", pFileName);
	}
	else
	{
		buffer = (char *)malloc(fileSize - 9);
		buffer[fileSize - 8] = '\0';
		if (buffer == nullptr)
			return 1;

		fread(buffer, sizeof(char), fileSize - 10, pFile);
		fclose(pFile);

		for (char *c = buffer; *c != '\0'; c++)
		{
			*c ^= MASK;
		}
		
		fopen_s(&pFile, pFileName, "wb");
		if (pFile == 0)
			return 1;

		fwrite(buffer, sizeof(char), fileSize - 10, pFile);

		printf("파일 %s의 복호화가 완료되었습니다.", pFileName);
	}
	return 0;
}