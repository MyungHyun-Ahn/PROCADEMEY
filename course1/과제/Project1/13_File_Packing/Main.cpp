#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_COUNT 10
#define MAX_PATH 256

#pragma pack(push, 1)
typedef struct _PACKING_INFO_HEADER
{
	unsigned const short	m_PackingCode = 0xABCD;
	unsigned short			m_fileCount = 0;
	unsigned int			m_headerSize = 0;
} PACKING_INFO_HEADER;

typedef struct _FILE_INFO_HEADER
{
	char				m_fileName[MAX_PATH];
	unsigned int		m_fileOffset;
	unsigned int		m_fileSize;
	
} FILE_INFO_HEADER;
#pragma pack(pop)

int main()
{
	// Packing
	FILE *files[MAX_FILE_COUNT];
	int fileCount = 0;

	PACKING_INFO_HEADER pih;
	FILE_INFO_HEADER fih[MAX_FILE_COUNT];
	char *buffers[MAX_FILE_COUNT];

	for (int i = 0; i < MAX_FILE_COUNT; i++)
	{
		scanf_s("%s", &fih[i].m_fileName, MAX_PATH);
		if (strcmp(fih[i].m_fileName, "-1") == 0) // 종료
			break;

		errno_t err = fopen_s(&files[i], fih[i].m_fileName, "rb");
		if (files[i] == 0)
		{
			printf("error - fopen_s : %s \n", fih[i].m_fileName);
			return 1;
		}

		fileCount++;
	}
	pih.m_fileCount = (unsigned short)fileCount;

	for (int i = 0; i < fileCount; i++)
	{

		fseek(files[i], 0, SEEK_END);
		fih[i].m_fileSize = ftell(files[i]);
		fseek(files[i], 0, SEEK_SET);
		buffers[i] = (char *)malloc(fih[i].m_fileSize);

		int readSize = fread_s(buffers[i], fih[i].m_fileSize, sizeof(char), fih[i].m_fileSize, files[i]);
		if (readSize != fih[i].m_fileSize)
		{
			printf("error - fread_s : %s \n", fih[i].m_fileName);
			return 1;
		}

		fclose(files[i]);
	}


	// 파일 시작 오프셋 계산
	int startOffset = sizeof(PACKING_INFO_HEADER) + sizeof(FILE_INFO_HEADER) * fileCount;
	pih.m_headerSize = startOffset;

	fih[0].m_fileOffset = startOffset;

	for (int i = 1; i < fileCount; i++)
	{
		fih[i].m_fileOffset = fih[i - 1].m_fileOffset + fih[i - 1].m_fileSize;
	}

	// 마지막 파일 오프셋 + 사이즈가 총 크기
	unsigned int packingBufferSize = fih[fileCount - 1].m_fileOffset + fih[fileCount - 1].m_fileSize;
	char *packingBuffer = (char *)malloc(packingBufferSize);
	if (packingBuffer == nullptr)
	{
		return 1;
	}

	unsigned int nextSize = sizeof(PACKING_INFO_HEADER);
	memcpy_s(packingBuffer, packingBufferSize, &pih, nextSize);

	unsigned int beforeSize = nextSize;
	nextSize = sizeof(FILE_INFO_HEADER) * fileCount;
	memcpy_s(packingBuffer + beforeSize, packingBufferSize - beforeSize, &fih, nextSize);

	beforeSize += nextSize;

	for (int i = 0; i < fileCount; i++)
	{
		memcpy_s(packingBuffer + fih[i].m_fileOffset, packingBufferSize - fih[i].m_fileOffset, buffers[i], fih[i].m_fileSize);
	}

	FILE *packingFile;
	fopen_s(&packingFile, "packing.dat", "wb");
	if (packingFile == 0)
		return 1;
	fwrite(packingBuffer, sizeof(char), packingBufferSize, packingFile);
	fclose(packingFile);

	return 0;
}