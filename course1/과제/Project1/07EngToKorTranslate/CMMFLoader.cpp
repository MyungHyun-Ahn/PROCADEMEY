#include "pch.h"
#include "CMMFLoader.h"

CMMFLoader::CMMFLoader(const TCHAR *filePath, size_t readBufferSize)
	: m_FilePath(filePath)
	, m_iReadBufferSize(readBufferSize)
{
	m_FileHandle = CreateFile(m_FilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	m_MMFHandle = CreateFileMapping(m_FileHandle, NULL, PAGE_WRITECOPY, 0, 0, NULL);
	if (m_MMFHandle == NULL)
	{
		_tprintf(TEXT("CreateFileMapping Error! Code : %d\n"), GetLastError());
	}
	// m_ReadBuffer = new TCHAR[readBufferSize];
}

CMMFLoader::~CMMFLoader()
{
	if (m_FileHandle != nullptr)
	{
		CloseHandle(m_FileHandle);
		m_FileHandle = nullptr;
	}

	if (m_MMFHandle != nullptr)
	{
		CloseHandle(m_MMFHandle);
		m_MMFHandle = nullptr;
	}
}

TCHAR *&&CMMFLoader::Load()
{
	m_ReadBuffer = (TCHAR *)MapViewOfFile(m_MMFHandle, FILE_MAP_COPY, 0, 0, 0);
	size_t readSize = _tcslen(m_ReadBuffer);
	m_ReadBuffer[readSize + 1] = '\0';
	return std::move(m_ReadBuffer);
}
