#include "pch.h"
#include "CFileLoader.h"

// 파일은 UTF-16으로 인코딩 해야 합니다.
CFileLoader::CFileLoader(const TCHAR *filePath, size_t readBufferSize) 
	: m_FilePath(filePath)
	, m_iReadBufferSize(readBufferSize)
{
	m_FileHandle = CreateFile(m_FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	m_ReadBuffer = new TCHAR[readBufferSize];
}

CFileLoader::~CFileLoader()
{
	if (m_FileHandle != nullptr)
	{
		CloseHandle(m_FileHandle);
		m_FileHandle = nullptr;
	}
}

// Load R-Value 참조로 받는 쪽에게 소유권 이전
TCHAR *&&CFileLoader::Load()
{
	DWORD readSize;
	if (!ReadFile(m_FileHandle, m_ReadBuffer, m_iReadBufferSize, &readSize, NULL))
	{
		_tprintf(TEXT("CFileLoader::Load %s 파일 읽기 실패"), m_FilePath);
		return nullptr;
	}

	m_ReadBuffer[(readSize / sizeof(TCHAR))] = '\0';

	return std::move(m_ReadBuffer);
}


