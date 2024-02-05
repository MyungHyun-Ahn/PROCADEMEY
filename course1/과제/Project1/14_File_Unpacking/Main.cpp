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
	FILE *packingFile;
	char packingFileName[MAX_PATH];
	scanf_s("%s", packingFileName, MAX_PATH);
	errno_t err = fopen_s(&packingFile, packingFileName, "rb");
	if (packingFile == 0)
		return 1;
	fseek(packingFile, 0, SEEK_END);
	int packingFileSize = ftell(packingFile);
	fseek(packingFile, 0, SEEK_SET);
	
	char *packingBuffer = (char *)malloc(packingFileSize);
	if (packingBuffer == nullptr)
		return 1;
	fread_s(packingBuffer, packingFileSize, sizeof(char), packingFileSize, packingFile);
	fclose(packingFile);

	PACKING_INFO_HEADER pih;

	memcpy_s(&pih, sizeof(PACKING_INFO_HEADER), packingBuffer, sizeof(PACKING_INFO_HEADER));

	FILE_INFO_HEADER *fihs = (FILE_INFO_HEADER *)malloc(sizeof(FILE_INFO_HEADER) * pih.m_fileCount);
	if (fihs == nullptr)
		return 1;
	memcpy_s(fihs, sizeof(FILE_INFO_HEADER) * pih.m_fileCount, packingBuffer + sizeof(PACKING_INFO_HEADER), sizeof(FILE_INFO_HEADER) * pih.m_fileCount);

	for (int i = 0; i < pih.m_fileCount; i++)
	{
		FILE *pFile;

		errno_t err = fopen_s(&pFile, fihs[i].m_fileName, "wb");
		if (pFile == nullptr)
			return 1;


		char *buffer = (char *)malloc(fihs[i].m_fileSize);
		if (buffer == nullptr)
			return 1;

		memcpy_s(buffer, fihs[i].m_fileSize, packingBuffer + fihs[i].m_fileOffset, fihs[i].m_fileSize);

		fwrite(buffer, sizeof(char), fihs[i].m_fileSize, pFile);
		fclose(pFile);
		free(buffer);
	}

	free(packingBuffer);

	return 0;
}